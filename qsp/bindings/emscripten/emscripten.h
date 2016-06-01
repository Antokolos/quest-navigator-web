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

#ifndef QSP_EMSCRIPTENDEFINES
	#define QSP_EMSCRIPTENDEFINES

#include "../../qsp.h"
#include "../../ConvertUTF.h"

	#ifdef _UNICODE
		typedef wchar_t QSP_CHAR;
	#endif

	#ifdef __cplusplus
		typedef int (*QSP_CALLBACK)(...);
	#else
		typedef int (*QSP_CALLBACK)();
	#endif

	#ifdef __cplusplus
	extern "C"
	{
	#endif

	QSP_EXTERN QSP_BOOL QSPIsInCallBack();
	QSP_EXTERN void QSPEnableDebugMode(QSP_BOOL isDebug);
	QSP_EXTERN void QSPGetCurStateData(QSP_CHAR **loc, int *actIndex, int *line);
	QSP_EXTERN const UTF8 *QSPGetVersion();
	QSP_EXTERN const QSP_CHAR *QSPGetCompiledDateTime();
	QSP_EXTERN int QSPGetFullRefreshCount();
	QSP_EXTERN const QSP_CHAR *QSPGetQstFullPath();
	QSP_EXTERN const QSP_CHAR *QSPGetCurLoc();
	QSP_EXTERN const QSP_CHAR *QSPGetMainDesc();
	QSP_EXTERN QSP_BOOL QSPIsMainDescChanged();
	QSP_EXTERN const QSP_CHAR *QSPGetVarsDesc();
	QSP_EXTERN QSP_BOOL QSPIsVarsDescChanged();
	QSP_EXTERN QSP_BOOL QSPGetExprValue(const QSP_CHAR *str, QSP_BOOL *isString, int *numVal, QSP_CHAR *strVal, int strValBufSize);
	QSP_EXTERN void QSPSetInputStrText(const QSP_CHAR *str);
	QSP_EXTERN int QSPGetActionsCount();
	QSP_EXTERN void QSPGetActionData(int ind, QSP_CHAR **imgPath, QSP_CHAR **desc);
	QSP_EXTERN QSP_BOOL QSPExecuteSelActionCode(QSP_BOOL isRefresh);
	QSP_EXTERN QSP_BOOL QSPSetSelActionIndex(int ind, QSP_BOOL isRefresh);
	QSP_EXTERN int QSPGetSelActionIndex();
	QSP_EXTERN QSP_BOOL QSPIsActionsChanged();
	QSP_EXTERN int QSPGetObjectsCount();
	QSP_EXTERN void QSPGetObjectData(int ind, QSP_CHAR **imgPath, QSP_CHAR **desc);
	QSP_EXTERN QSP_BOOL QSPSetSelObjectIndex(int ind, QSP_BOOL isRefresh);
	QSP_EXTERN int QSPGetSelObjectIndex();
	QSP_EXTERN QSP_BOOL QSPIsObjectsChanged();
	QSP_EXTERN void QSPShowWindow(int type, QSP_BOOL isShow);
	QSP_EXTERN QSP_BOOL QSPGetVarValuesCount(const QSP_CHAR *name, int *count);
	QSP_EXTERN QSP_BOOL QSPGetVarValues(const QSP_CHAR *name, int ind, int *numVal, QSP_CHAR **strVal);
	QSP_EXTERN int QSPGetMaxVarsCount();
	QSP_EXTERN QSP_BOOL QSPGetVarNameByIndex(int ind, QSP_CHAR **name);
	QSP_EXTERN QSP_BOOL QSPExecString(const QSP_CHAR *str, QSP_BOOL isRefresh);
	QSP_EXTERN QSP_BOOL QSPExecCounter(QSP_BOOL isRefresh);
	QSP_EXTERN QSP_BOOL QSPExecUserInput(QSP_BOOL isRefresh);
	QSP_EXTERN QSP_BOOL QSPExecLocationCode(const QSP_CHAR *name, QSP_BOOL isRefresh);
	QSP_EXTERN void QSPGetLastErrorData(int *errorNum, QSP_CHAR **errorLoc, int *errorActIndex, int *errorLine);
	QSP_EXTERN const QSP_CHAR *QSPGetErrorDesc(int errorNum);
	QSP_EXTERN QSP_BOOL QSPLoadGameWorld(const QSP_CHAR *file);
	QSP_EXTERN QSP_BOOL QSPLoadGameWorldFromData(const void *data, int dataSize, const QSP_CHAR *file);
	QSP_EXTERN QSP_BOOL QSPSaveGame(const QSP_CHAR *file, QSP_BOOL isRefresh);
	QSP_EXTERN QSP_BOOL QSPSaveGameAsData(void *buf, int bufSize, int *realSize, QSP_BOOL isRefresh);
	QSP_EXTERN QSP_BOOL QSPOpenSavedGame(const QSP_CHAR *file, QSP_BOOL isRefresh);
	QSP_EXTERN QSP_BOOL QSPOpenSavedGameFromData(const void *data, int dataSize, QSP_BOOL isRefresh);
	QSP_EXTERN QSP_BOOL QSPRestartGame(QSP_BOOL isRefresh);
	QSP_EXTERN void QSPSelectMenuItem(int ind);
	QSP_EXTERN void QSPSetCallBack(int type, QSP_CALLBACK func);
	QSP_EXTERN void QSPInit();
	QSP_EXTERN void QSPDeInit();

	//QSP_EXTERN QSP_BOOL QSPIsInCallBackImpl();
	//QSP_EXTERN void QSPEnableDebugModeImpl(QSP_BOOL isDebug);
	//QSP_EXTERN void QSPGetCurStateDataImpl(QSP_CHAR **loc, int *actIndex, int *line);
	QSP_EXTERN const QSP_CHAR *QSPGetVersionImpl();
	/*QSP_EXTERN const QSP_CHAR *QSPGetCompiledDateTimeImpl();
	QSP_EXTERN int QSPGetFullRefreshCountImpl();
	QSP_EXTERN const QSP_CHAR *QSPGetQstFullPathImpl();
	QSP_EXTERN const QSP_CHAR *QSPGetCurLocImpl();
	QSP_EXTERN const QSP_CHAR *QSPGetMainDescImpl();
	QSP_EXTERN QSP_BOOL QSPIsMainDescChangedImpl();
	QSP_EXTERN const QSP_CHAR *QSPGetVarsDescImpl();
	QSP_EXTERN QSP_BOOL QSPIsVarsDescChangedImpl();
	QSP_EXTERN QSP_BOOL QSPGetExprValueImpl(const QSP_CHAR *str, QSP_BOOL *isString, int *numVal, QSP_CHAR *strVal, int strValBufSize);
	QSP_EXTERN void QSPSetInputStrTextImpl(const QSP_CHAR *str);
	QSP_EXTERN int QSPGetActionsCountImpl();
	QSP_EXTERN void QSPGetActionDataImpl(int ind, QSP_CHAR **imgPath, QSP_CHAR **desc);
	QSP_EXTERN QSP_BOOL QSPExecuteSelActionCodeImpl(QSP_BOOL isRefresh);
	QSP_EXTERN QSP_BOOL QSPSetSelActionIndexImpl(int ind, QSP_BOOL isRefresh);
	QSP_EXTERN int QSPGetSelActionIndexImpl();
	QSP_EXTERN QSP_BOOL QSPIsActionsChangedImpl();
	QSP_EXTERN int QSPGetObjectsCountImpl();
	QSP_EXTERN void QSPGetObjectDataImpl(int ind, QSP_CHAR **imgPath, QSP_CHAR **desc);
	QSP_EXTERN QSP_BOOL QSPSetSelObjectIndexImpl(int ind, QSP_BOOL isRefresh);
	QSP_EXTERN int QSPGetSelObjectIndexImpl();
	QSP_EXTERN QSP_BOOL QSPIsObjectsChangedImpl();
	QSP_EXTERN void QSPShowWindowImpl(int type, QSP_BOOL isShow);
	QSP_EXTERN QSP_BOOL QSPGetVarValuesCountImpl(const QSP_CHAR *name, int *count);
	QSP_EXTERN QSP_BOOL QSPGetVarValuesImpl(const QSP_CHAR *name, int ind, int *numVal, QSP_CHAR **strVal);
	QSP_EXTERN int QSPGetMaxVarsCountImpl();
	QSP_EXTERN QSP_BOOL QSPGetVarNameByIndexImpl(int ind, QSP_CHAR **name);
	QSP_EXTERN QSP_BOOL QSPExecStringImpl(const QSP_CHAR *str, QSP_BOOL isRefresh);
	QSP_EXTERN QSP_BOOL QSPExecCounterImpl(QSP_BOOL isRefresh);
	QSP_EXTERN QSP_BOOL QSPExecUserInputImpl(QSP_BOOL isRefresh);
	QSP_EXTERN QSP_BOOL QSPExecLocationCodeImpl(const QSP_CHAR *name, QSP_BOOL isRefresh);
	QSP_EXTERN void QSPGetLastErrorDataImpl(int *errorNum, QSP_CHAR **errorLoc, int *errorActIndex, int *errorLine);
	QSP_EXTERN const QSP_CHAR *QSPGetErrorDescImpl(int errorNum);
	QSP_EXTERN QSP_BOOL QSPLoadGameWorldImpl(const QSP_CHAR *file);
	QSP_EXTERN QSP_BOOL QSPLoadGameWorldFromDataImpl(const void *data, int dataSize, const QSP_CHAR *file);
	QSP_EXTERN QSP_BOOL QSPSaveGameImpl(const QSP_CHAR *file, QSP_BOOL isRefresh);
	QSP_EXTERN QSP_BOOL QSPSaveGameAsDataImpl(void *buf, int bufSize, int *realSize, QSP_BOOL isRefresh);
	QSP_EXTERN QSP_BOOL QSPOpenSavedGameImpl(const QSP_CHAR *file, QSP_BOOL isRefresh);
	QSP_EXTERN QSP_BOOL QSPOpenSavedGameFromDataImpl(const void *data, int dataSize, QSP_BOOL isRefresh);
	QSP_EXTERN QSP_BOOL QSPRestartGameImpl(QSP_BOOL isRefresh);
	QSP_EXTERN void QSPSelectMenuItemImpl(int ind);
	QSP_EXTERN void QSPSetCallBackImpl(int type, QSP_CALLBACK func);
	QSP_EXTERN void QSPInitImpl();
	QSP_EXTERN void QSPDeInitImpl();*/
	
	/* Utils */
	QSP_EXTERN const UTF8 *QSPCharToUTF8(QSP_CHAR *source);
	#ifdef __cplusplus
	}
	#endif

#endif
