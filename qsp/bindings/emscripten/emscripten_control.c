/* Copyright (C) 2005-2010 Valeriy Argunov (nporep AT mail DOT ru) */
/*
* This library is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 2.1 of the License, or
* (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#include "../../declarations.h"

#ifdef _EMSCRIPTEN

#include "../../actions.h"
#include "../../callbacks.h"
#include "../../coding.h"
#include "../../common.h"
#include "../../errors.h"
#include "../../game.h"
#include "../../locations.h"
#include "../../mathops.h"
#include "../../menu.h"
#include "../../objects.h"
#include "../../statements.h"
#include "../../text.h"
#include "../../time.h"
#include "../../variables.h"
#include "../../variant.h"

/* ------------------------------------------------------------ */
QSP_BOOL QSPIsInCallBack()
{
	return qspIsInCallBack;
}
/* ------------------------------------------------------------ */
/* ������� */

/* ���������� ������� ������� */
void QSPEnableDebugMode(QSP_BOOL isDebug)
{
	qspIsDebug = isDebug;
}
/* ��������� ������ �������� ��������� */
QSPCurStateData QSPGetCurStateDataW()
{
	QSPCurStateData result;
	QSP_CHAR* loc;
	int actIndex;
	int line;
	QSPGetCurStateData(&loc, &actIndex, &line);
	result.loc = QSPCharToUTF8(loc);
	result.actIndex = actIndex;
	result.line = line;
	return result;
}
void QSPGetCurStateData(QSP_CHAR **loc, int *actIndex, int *line)
{
	*loc = (qspRealCurLoc >= 0 && qspRealCurLoc < qspLocsCount ? qspLocs[qspRealCurLoc].Name : 0);
	*actIndex = qspRealActIndex;
	*line = qspRealLine;
}
/* ------------------------------------------------------------ */
/* ���������� � ������ */

/* ������ */

const UTF8 *QSPGetVersionW()
{
	return QSPCharToUTF8(QSPGetVersion());
}
const QSP_CHAR *QSPGetVersion()
{
	return QSP_VER;
}
/* ���� � ����� ���������� */
const UTF8 *QSPGetCompiledDateTimeW()
{
	return QSPCharToUTF8(QSPGetCompiledDateTime());
}
const QSP_CHAR *QSPGetCompiledDateTime()
{
	return QSP_FMT(__DATE__) QSP_FMT(", ") QSP_FMT(__TIME__);
}
/* ------------------------------------------------------------ */
/* ���������� ������ ���������� ������� */
int QSPGetFullRefreshCount()
{
	return qspFullRefreshCount;
}
/* ------------------------------------------------------------ */
/* ������ ���� � ������������ ����� ���� */
const UTF8 *QSPGetQstFullPathW()
{
	return QSPCharToUTF8(QSPGetQstFullPath());
}
const QSP_CHAR *QSPGetQstFullPath()
{
	return qspQstFullPath;
}
/* ------------------------------------------------------------ */
/* �������� ������� ������� */
const UTF8 *QSPGetCurLocW()
{
	return QSPCharToUTF8(QSPGetCurLoc());
}
const QSP_CHAR *QSPGetCurLoc()
{
	return (qspCurLoc >= 0 ? qspLocs[qspCurLoc].Name : 0);
}
/* ------------------------------------------------------------ */
/* �������� �������� ������� */

/* ����� ��������� ���� �������� ������� */
const UTF8 *QSPGetMainDescW()
{
	return QSPCharToUTF8(QSPGetMainDesc());
}
const QSP_CHAR *QSPGetMainDesc()
{
	return qspCurDesc;
}
/* ����������� ��������� ������ ��������� �������� */
QSP_BOOL QSPIsMainDescChanged()
{
	return qspIsMainDescChanged;
}
/* ------------------------------------------------------------ */
/* �������������� �������� ������� */

/* ����� ��������������� ���� �������� ������� */
const UTF8 *QSPGetVarsDescW()
{
	return QSPCharToUTF8(QSPGetVarsDesc());
}
const QSP_CHAR *QSPGetVarsDesc()
{
	return qspCurVars;
}
/* ����������� ��������� ������ ��������������� �������� */
QSP_BOOL QSPIsVarsDescChanged()
{
	return qspIsVarsDescChanged;
}
/* ------------------------------------------------------------ */
/* �������� �������� ���������� ��������� */
QSPExprValue QSPGetExprValueW(const UTF8 *expr, int strValBufSize)
{
	QSPExprValue result;
	QSP_CHAR* strVal = malloc(strValBufSize * sizeof(QSP_CHAR));
	QSP_BOOL status = QSPGetExprValue(UTF8ToQSPChar(expr), &(result.isString), &(result.numVal), strVal, strValBufSize);
	result.status = status;
	result.strVal = QSPCharToUTF8(strVal);
	return result;
}

QSP_BOOL QSPGetExprValue(const QSP_CHAR *expr, QSP_BOOL *isString, int *numVal, QSP_CHAR *strVal, int strValBufSize)
{
	QSPVariant v;
	if (qspIsExitOnError && qspErrorNum) return QSP_FALSE;
	qspResetError();
	if (qspIsDisableCodeExec) return QSP_FALSE;
	v = qspExprValue((QSP_CHAR *)expr);
	if (qspErrorNum) return QSP_FALSE;
	*isString = v.IsStr;
	if (v.IsStr)
	{
		qspStrNCopy(strVal, QSP_STR(v), strValBufSize - 1);
		free(QSP_STR(v));
		strVal[strValBufSize - 1] = 0;
	}
	else
		*numVal = QSP_NUM(v);
	return QSP_TRUE;
}
/* ------------------------------------------------------------ */
/* ����� ������ ����� */
void QSPSetInputStrTextW(const UTF8 *val)
{
	QSPSetInputStrText(UTF8ToQSPChar(val));
}

void QSPSetInputStrText(const QSP_CHAR *val)
{
	qspCurInputLen = qspAddText(&qspCurInput, (QSP_CHAR *)val, 0, -1, QSP_FALSE);
}
/* ------------------------------------------------------------ */
/* ������ �������� */

/* ���������� �������� */
int QSPGetActionsCount()
{
	return qspCurActionsCount;
}
/* ������ �������� � ��������� �������� */
QSPActionData QSPGetActionDataW(int ind)
{
	QSPActionData result;
	QSP_CHAR* image;
	QSP_CHAR* desc;
	QSPGetActionData(ind, &image, &desc);
	result.imgPath = QSPCharToUTF8(image);
	result.desc = QSPCharToUTF8(desc);
	return result;
}

void QSPGetActionData(int ind, QSP_CHAR **image, QSP_CHAR **desc)
{
	if (ind >= 0 && ind < qspCurActionsCount)
	{
		*image = qspCurActions[ind].Image;
		*desc = qspCurActions[ind].Desc;
	}
	else
		*image = *desc = 0;
}
/* ���������� ���� ���������� �������� */
QSP_BOOL QSPExecuteSelActionCode(QSP_BOOL isRefresh)
{
	if (qspCurSelAction >= 0)
	{
		if (qspIsExitOnError && qspErrorNum) return QSP_FALSE;
		qspPrepareExecution();
		if (qspIsDisableCodeExec) return QSP_FALSE;
		qspExecAction(qspCurSelAction);
		if (qspErrorNum) return QSP_FALSE;
		if (isRefresh) qspCallRefreshInt(QSP_FALSE);
	}
	return QSP_TRUE;
}
/* ���������� ������ ���������� �������� */
QSP_BOOL QSPSetSelActionIndex(int ind, QSP_BOOL isRefresh)
{
	if (ind >= 0 && ind < qspCurActionsCount && ind != qspCurSelAction)
	{
		if (qspIsExitOnError && qspErrorNum) return QSP_FALSE;
		qspPrepareExecution();
		if (qspIsDisableCodeExec) return QSP_FALSE;
		qspCurSelAction = ind;
		qspExecLocByVarNameWithArgs(QSP_FMT("ONACTSEL"), 0, 0);
		if (qspErrorNum) return QSP_FALSE;
		if (isRefresh) qspCallRefreshInt(QSP_FALSE);
	}
	return QSP_TRUE;
}
/* �������� ������ ���������� �������� */
int QSPGetSelActionIndex()
{
	return qspCurSelAction;
}
/* ����������� ��������� ������ �������� */
QSP_BOOL QSPIsActionsChanged()
{
	return qspIsActionsChanged;
}
/* ------------------------------------------------------------ */
/* ������ �������� */

/* ���������� �������� */
int QSPGetObjectsCount()
{
	return qspCurObjectsCount;
}
/* ������ ������� � ��������� �������� */
QSPObjectData QSPGetObjectDataW(int ind)
{
	QSPObjectData result;
	QSP_CHAR* image;
	QSP_CHAR* desc;
	QSPGetObjectData(ind, &image, &desc);
	result.imgPath = QSPCharToUTF8(image);
	result.desc = QSPCharToUTF8(desc);
	return result;
}

void QSPGetObjectData(int ind, QSP_CHAR **image, QSP_CHAR **desc)
{
	if (ind >= 0 && ind < qspCurObjectsCount)
	{
		*image = qspCurObjects[ind].Image;
		*desc = qspCurObjects[ind].Desc;
	}
	else
		*image = *desc = 0;
}
/* ���������� ������ ���������� ������� */
QSP_BOOL QSPSetSelObjectIndex(int ind, QSP_BOOL isRefresh)
{
	if (ind >= 0 && ind < qspCurObjectsCount && ind != qspCurSelObject)
	{
		if (qspIsExitOnError && qspErrorNum) return QSP_FALSE;
		qspPrepareExecution();
		if (qspIsDisableCodeExec) return QSP_FALSE;
		qspCurSelObject = ind;
		qspExecLocByVarNameWithArgs(QSP_FMT("ONOBJSEL"), 0, 0);
		if (qspErrorNum) return QSP_FALSE;
		if (isRefresh) qspCallRefreshInt(QSP_FALSE);
	}
	return QSP_TRUE;
}
/* �������� ������ ���������� ������� */
int QSPGetSelObjectIndex()
{
	return qspCurSelObject;
}
/* ����������� ��������� ������ �������� */
QSP_BOOL QSPIsObjectsChanged()
{
	return qspIsObjectsChanged;
}
/* ------------------------------------------------------------ */
/* ����� / ������� ���� */
void QSPShowWindow(int type, QSP_BOOL isShow)
{
	switch (type)
	{
	case QSP_WIN_ACTS:
		qspCurIsShowActs = isShow;
		break;
	case QSP_WIN_OBJS:
		qspCurIsShowObjs = isShow;
		break;
	case QSP_WIN_VARS:
		qspCurIsShowVars = isShow;
		break;
	case QSP_WIN_INPUT:
		qspCurIsShowInput = isShow;
		break;
	}
}
/* ------------------------------------------------------------ */
/* ���������� */

/* �������� ���������� ��������� ������� */
QSPVarValuesCountData QSPGetVarValuesCountW(const UTF8 *name)
{
	QSPVarValuesCountData result;
	QSP_BOOL status = QSPGetVarValuesCount(UTF8ToQSPChar(name), &(result.count));
	result.status = status;
	return result;
}

QSP_BOOL QSPGetVarValuesCount(const QSP_CHAR *name, int *count)
{
	QSPVar *var;
	if (qspIsExitOnError && qspErrorNum) return QSP_FALSE;
	qspResetError();
	var = qspVarReference((QSP_CHAR *)name, QSP_FALSE);
	if (qspErrorNum) return QSP_FALSE;
	*count = var->ValsCount;
	return QSP_TRUE;
}
/* �������� �������� ���������� �������� ������� */
QSPVarValuesData QSPGetVarValuesW(const UTF8 *name, int ind)
{
	QSPVarValuesData result;
	QSP_CHAR* strVal;
	QSP_BOOL status = QSPGetVarValues(UTF8ToQSPChar(name), ind, &(result.numVal), &strVal);
	result.status = status;
	result.strVal = QSPCharToUTF8(strVal);
	return result;
}

QSP_BOOL QSPGetVarValues(const QSP_CHAR *name, int ind, int *numVal, QSP_CHAR **strVal)
{
	QSPVar *var;
	if (qspIsExitOnError && qspErrorNum) return QSP_FALSE;
	qspResetError();
	var = qspVarReference((QSP_CHAR *)name, QSP_FALSE);
	if (qspErrorNum || ind < 0 || ind >= var->ValsCount) return QSP_FALSE;
	*numVal = var->Values[ind].Num;
	*strVal = var->Values[ind].Str;
	return QSP_TRUE;
}
/* �������� ������������ ���������� ���������� */
int QSPGetMaxVarsCount()
{
	return QSP_VARSCOUNT;
}
/* �������� ��� ���������� � ��������� �������� */
QSPVarNameData QSPGetVarNameByIndexW(int index)
{
	QSPVarNameData result;
	QSP_CHAR* name;
	QSP_BOOL status = QSPGetVarNameByIndex(index, &name);
	result.status = status;
	result.name = QSPCharToUTF8(name);
	return result;
}

QSP_BOOL QSPGetVarNameByIndex(int index, QSP_CHAR **name)
{
	if (index < 0 || index >= QSP_VARSCOUNT || !qspVars[index].Name) return QSP_FALSE;
	*name = qspVars[index].Name;
	return QSP_TRUE;
}
/* ------------------------------------------------------------ */
/* ���������� ���� */

/* ���������� ������ ���� */
QSP_BOOL QSPExecStringW(const UTF8 *s, QSP_BOOL isRefresh)
{
	return QSPExecString(UTF8ToQSPChar(s), isRefresh);
}

QSP_BOOL QSPExecString(const QSP_CHAR *s, QSP_BOOL isRefresh)
{
	if (qspIsExitOnError && qspErrorNum) return QSP_FALSE;
	qspPrepareExecution();
	if (qspIsDisableCodeExec) return QSP_FALSE;
	qspExecStringAsCodeWithArgs((QSP_CHAR *)s, 0, 0);
	if (qspErrorNum) return QSP_FALSE;
	if (isRefresh) qspCallRefreshInt(QSP_FALSE);
	return QSP_TRUE;
}
/* ���������� ���� ��������� ������� */
QSP_BOOL QSPExecLocationCodeW(const UTF8 *name, QSP_BOOL isRefresh)
{
	return QSPExecLocationCode(UTF8ToQSPChar(name), isRefresh);
}

QSP_BOOL QSPExecLocationCode(const QSP_CHAR *name, QSP_BOOL isRefresh)
{
	if (qspIsExitOnError && qspErrorNum) return QSP_FALSE;
	qspPrepareExecution();
	if (qspIsDisableCodeExec) return QSP_FALSE;
	qspExecLocByName((QSP_CHAR *)name, QSP_FALSE);
	if (qspErrorNum) return QSP_FALSE;
	if (isRefresh) qspCallRefreshInt(QSP_FALSE);
	return QSP_TRUE;
}
/* ���������� ���� �������-�������� */
QSP_BOOL QSPExecCounter(QSP_BOOL isRefresh)
{
	if (!qspIsInCallBack)
	{
		qspPrepareExecution();
		qspExecLocByVarNameWithArgs(QSP_FMT("COUNTER"), 0, 0);
		if (qspErrorNum) return QSP_FALSE;
		if (isRefresh) qspCallRefreshInt(QSP_FALSE);
	}
	return QSP_TRUE;
}
/* ���������� ���� �������-����������� ������ ����� */
QSP_BOOL QSPExecUserInput(QSP_BOOL isRefresh)
{
	if (qspIsExitOnError && qspErrorNum) return QSP_FALSE;
	qspPrepareExecution();
	if (qspIsDisableCodeExec) return QSP_FALSE;
	qspExecLocByVarNameWithArgs(QSP_FMT("USERCOM"), 0, 0);
	if (qspErrorNum) return QSP_FALSE;
	if (isRefresh) qspCallRefreshInt(QSP_FALSE);
	return QSP_TRUE;
}
/* ------------------------------------------------------------ */
/* ������ */

/* �������� ���������� � ��������� ������ */
QSPLastErrorData QSPGetLastErrorDataW()
{
	QSPLastErrorData result;
	QSP_CHAR* errorLoc;
	QSPGetLastErrorData(&(result.errorNum), &errorLoc, &(result.errorActIndex), &(result.errorLine));
	result.errorLoc = QSPCharToUTF8(errorLoc);
	return result;
}

void QSPGetLastErrorData(int *errorNum, QSP_CHAR **errorLoc, int *errorActIndex, int *errorLine)
{
	*errorNum = qspErrorNum;
	*errorLoc = (qspErrorLoc >= 0 && qspErrorLoc < qspLocsCount ? qspLocs[qspErrorLoc].Name : 0);
	*errorActIndex = qspErrorActIndex;
	*errorLine = qspErrorLine;
}
/* �������� �������� ������ �� �� ������ */
const UTF8 *QSPGetErrorDescW(int errorNum)
{
	return QSPCharToUTF8(QSPGetErrorDesc(errorNum));
}

const QSP_CHAR *QSPGetErrorDesc(int errorNum)
{
	return qspGetErrorDesc(errorNum);
}
/* ------------------------------------------------------------ */
/* ���������� ����� */

/* �������� ����� ���� �� ����� */
QSP_BOOL QSPLoadGameWorldW(const UTF8 *fileName)
{
	return QSPLoadGameWorld(UTF8ToQSPChar(fileName));
}

QSP_BOOL QSPLoadGameWorld(const QSP_CHAR *fileName)
{
	if (qspIsExitOnError && qspErrorNum) return QSP_FALSE;
	qspResetError();
	if (qspIsDisableCodeExec) return QSP_FALSE;
	qspOpenQuest((QSP_CHAR *)fileName, QSP_FALSE);
	if (qspErrorNum) return QSP_FALSE;
	return QSP_TRUE;
}
/* �������� ����� ���� �� ������ */
QSP_BOOL QSPLoadGameWorldFromDataW(const void *data, int dataSize, const UTF8 *fileName)
{
	return QSPLoadGameWorldFromData(data, dataSize, UTF8ToQSPChar(fileName));
}

QSP_BOOL QSPLoadGameWorldFromData(const void *data, int dataSize, const QSP_CHAR *fileName)
{
	char *ptr;
	if (qspIsExitOnError && qspErrorNum) return QSP_FALSE;
	qspResetError();
	if (qspIsDisableCodeExec) return QSP_FALSE;
	ptr = (char *)malloc(dataSize + 3);
	memcpy(ptr, data, dataSize);
	ptr[dataSize] = ptr[dataSize + 1] = ptr[dataSize + 2] = 0;
	qspOpenQuestFromData(ptr, dataSize + 3, (QSP_CHAR *)fileName, QSP_FALSE);
	free(ptr);
	if (qspErrorNum) return QSP_FALSE;
	return QSP_TRUE;
}
/* ���������� ��������� � ���� */
QSP_BOOL QSPSaveGameW(const UTF8 *fileName, QSP_BOOL isRefresh)
{
	return QSPSaveGame(UTF8ToQSPChar(fileName), isRefresh);
}

QSP_BOOL QSPSaveGame(const QSP_CHAR *fileName, QSP_BOOL isRefresh)
{
	if (qspIsExitOnError && qspErrorNum) return QSP_FALSE;
	qspPrepareExecution();
	if (qspIsDisableCodeExec) return QSP_FALSE;
	qspSaveGameStatus((QSP_CHAR *)fileName);
	if (qspErrorNum) return QSP_FALSE;
	if (isRefresh) qspCallRefreshInt(QSP_FALSE);
	return QSP_TRUE;
}
/* ���������� ��������� � ������ */
QSPSaveGameData QSPGetFailedSaveData()
{	
	QSPSaveGameData result;
	result.status = QSP_FALSE;
	result.data = 0;
	return result;
}

QSPSaveGameData QSPSaveGameAsDataW(QSP_BOOL isRefresh)
{
	QSPSaveGameData result;
	int len;
	QSP_CHAR *data;
	if (qspIsExitOnError && qspErrorNum) return QSPGetFailedSaveData();
	qspPrepareExecution();
	if (qspIsDisableCodeExec) return QSPGetFailedSaveData();
	if (!(len = qspSaveGameStatusToString(&data)))
	{
		return QSPGetFailedSaveData();
	}
	result.data = QSPCharToUTF8(data);
	free(data);
	if (isRefresh) qspCallRefreshInt(QSP_FALSE);
	return result;
}

QSP_BOOL QSPSaveGameAsData(void *buf, int bufSize, int *realSize, QSP_BOOL isRefresh)
{
	int len, size;
	QSP_CHAR *data;
	if (qspIsExitOnError && qspErrorNum) return QSP_FALSE;
	qspPrepareExecution();
	if (qspIsDisableCodeExec) return QSP_FALSE;
	if (!(len = qspSaveGameStatusToString(&data)))
	{
		*realSize = 0;
		return QSP_FALSE;
	}
	size = len * sizeof(QSP_CHAR);
	*realSize = size;
	if (size > bufSize)
	{
		free(data);
		return QSP_FALSE;
	}
	memcpy(buf, data, size);
	free(data);
	if (isRefresh) qspCallRefreshInt(QSP_FALSE);
	return QSP_TRUE;
}
/* �������� ��������� �� ����� */
QSP_BOOL QSPOpenSavedGameW(const UTF8 *fileName, QSP_BOOL isRefresh)
{
	QSPOpenSavedGame(UTF8ToQSPChar(fileName), isRefresh);
}

QSP_BOOL QSPOpenSavedGame(const QSP_CHAR *fileName, QSP_BOOL isRefresh)
{
	if (qspIsExitOnError && qspErrorNum) return QSP_FALSE;
	qspPrepareExecution();
	if (qspIsDisableCodeExec) return QSP_FALSE;
	qspOpenGameStatus((QSP_CHAR *)fileName);
	if (qspErrorNum) return QSP_FALSE;
	if (isRefresh) qspCallRefreshInt(QSP_FALSE);
	return QSP_TRUE;
}
/* �������� ��������� �� ������ */
QSP_BOOL QSPOpenSavedGameFromDataW(const UTF8 *data, QSP_BOOL isRefresh)
{
	QSP_CHAR *ptr;
	if (qspIsExitOnError && qspErrorNum) return QSP_FALSE;
	qspPrepareExecution();
	if (qspIsDisableCodeExec) return QSP_FALSE;
	ptr = UTF8ToQSPChar(data);
	qspOpenGameStatusFromString(ptr);
	free(ptr);
	if (qspErrorNum) return QSP_FALSE;
	if (isRefresh) qspCallRefreshInt(QSP_FALSE);
	return QSP_TRUE;
}

QSP_BOOL QSPOpenSavedGameFromData(const void *data, int dataSize, QSP_BOOL isRefresh)
{
	int dataLen;
	QSP_CHAR *ptr;
	if (qspIsExitOnError && qspErrorNum) return QSP_FALSE;
	qspPrepareExecution();
	if (qspIsDisableCodeExec) return QSP_FALSE;
	dataLen = dataSize / sizeof(QSP_CHAR);
	ptr = (QSP_CHAR *)malloc((dataLen + 1) * sizeof(QSP_CHAR));
	memcpy(ptr, data, dataSize);
	ptr[dataLen] = 0;
	qspOpenGameStatusFromString(ptr);
	free(ptr);
	if (qspErrorNum) return QSP_FALSE;
	if (isRefresh) qspCallRefreshInt(QSP_FALSE);
	return QSP_TRUE;
}
/* ���������� ���� */
QSP_BOOL QSPRestartGame(QSP_BOOL isRefresh)
{
	if (qspIsExitOnError && qspErrorNum) return QSP_FALSE;
	qspPrepareExecution();
	if (qspIsDisableCodeExec) return QSP_FALSE;
	qspNewGame(QSP_TRUE);
	if (qspErrorNum) return QSP_FALSE;
	if (isRefresh) qspCallRefreshInt(QSP_FALSE);
	return QSP_TRUE;
}
extern int qspCurMenuItems;
extern QSP_CHAR *qspCurMenuLocs;
/* ------------------------------------------------------------ */
/* ���� */
/* �-� ������������� ������ ��� ������ �� CallBack'� QSP_CALL_SHOWMENU */
void QSPSelectMenuItem(int index)
{
	QSPVariant arg;
	if (index >= 0 && index < qspCurMenuItems)
	{
		if (qspIsDisableCodeExec) return;
		arg.IsStr = QSP_FALSE;
		QSP_NUM(arg) = index + 1;
		qspExecLocByNameWithArgs(qspCurMenuLocs[index], &arg, 1);
	}
}
/* ------------------------------------------------------------ */
/* ��������� CALLBACK'�� */
void QSPSetCallBack(int type, QSP_CALLBACK func)
{
	qspSetCallBack(type, func);
}
/* ------------------------------------------------------------ */
/* ������������� */
void QSPInit()
{
	#ifdef _DEBUG
		mwInit();
	#endif
	qspIsDebug = QSP_FALSE;
	qspRefreshCount = qspFullRefreshCount = 0;
	qspQstPath = qspQstFullPath = 0;
	qspQstPathLen = 0;
	qspQstCRC = 0;
	qspRealCurLoc = -1;
	qspRealActIndex = -1;
	qspRealLine = 0;
	qspMSCount = 0;
	qspLocs = 0;
	qspLocsNames = 0;
	qspLocsCount = 0;
	qspCurLoc = -1;
	qspTimerInterval = 0;
	qspCurIsShowObjs = qspCurIsShowActs = qspCurIsShowVars = qspCurIsShowInput = QSP_TRUE;
	setlocale(LC_ALL, QSP_LOCALE);
	qspSetSeed(0);
	qspPrepareExecution();
	qspMemClear(QSP_TRUE);
	qspInitCallBacks();
	qspInitStats();
	qspInitMath();
}
/* ��������������� */
void QSPDeInit()
{
	qspMemClear(QSP_FALSE);
	qspCreateWorld(0, 0);
	if (qspQstPath) free(qspQstPath);
	if (qspQstFullPath) free(qspQstFullPath);
	#ifdef _DEBUG
		mwTerm();
	#endif
}

#endif
