IDIR1 = qsp
IDIR2 = qsp/onig
IDIR3 = qsp/bindings
IDIR4 = qsp/bindings/emscripten
IDIR5 = qsp/onig/enc
#IDIR5 = qsp/bindings/android
#IDIR6 = qsp/bindings/flash
#IDIR7 = qsp/bindings/java
CC = emcc
CFLAGS = -I$(IDIR1) -I$(IDIR2) -I$(IDIR3) -I$(IDIR4) -fshort-wchar -D_UNICODE -DNOT_RUBY -DEXPORT -D_EMSCRIPTEN
QCFLAGS = -O2 -s EXPORTED_FUNCTIONS="[\
'_QSPIsInCallBack', \
'_QSPEnableDebugMode', \
'_QSPGetCurStateDataW', \
'_QSPGetVersionW', \
'_QSPGetCompiledDateTimeW', \
'_QSPGetFullRefreshCount', \
'_QSPGetQstFullPathW', \
'_QSPGetCurLocW', \
'_QSPGetMainDescW', \
'_QSPIsMainDescChanged', \
'_QSPGetVarsDescW', \
'_QSPIsVarsDescChanged', \
'_QSPGetExprValue', \
'_QSPSetInputStrText', \
'_QSPGetActionsCount', \
'_QSPGetActionData', \
'_QSPExecuteSelActionCode', \
'_QSPSetSelActionIndex', \
'_QSPGetSelActionIndex', \
'_QSPIsActionsChanged', \
'_QSPGetObjectsCount', \
'_QSPGetObjectData', \
'_QSPSetSelObjectIndex', \
'_QSPGetSelObjectIndex', \
'_QSPIsObjectsChanged', \
'_QSPShowWindow', \
'_QSPGetVarValuesCount', \
'_QSPGetVarValues', \
'_QSPGetMaxVarsCount', \
'_QSPGetVarNameByIndex', \
'_QSPExecString', \
'_QSPExecCounter', \
'_QSPExecUserInput', \
'_QSPExecLocationCode', \
'_QSPGetLastErrorData', \
'_QSPGetErrorDesc', \
'_QSPLoadGameWorld', \
'_QSPLoadGameWorldFromData', \
'_QSPSaveGame', \
'_QSPSaveGameAsData', \
'_QSPOpenSavedGame', \
'_QSPOpenSavedGameFromData', \
'_QSPRestartGame', \
'_QSPSelectMenuItem', \
'_QSPSetCallBack', \
'_QSPInit', \
'_QSPDeInit'\
]"

ODIR1 = obj
ODIR2 = obj/onig
ODIR3 = obj/bindings
ODIR4 = obj/bindings/emscripten
ODIR5 = obj/onig/enc
#LDIR = lib

#LIBS = -lm
LIBS =

_DEPS1 = ConvertUTF.h actions.h codetools.h common.h errors.h locations.h memwatch.h objects.h qsp.h text.h variables.h callbacks.h coding.h declarations.h game.h mathops.h menu.h playlist.h statements.h time.h variant.h
DEPS1 = $(patsubst %,$(IDIR1)/%,$(_DEPS1))

_DEPS2 = config.h oniguruma.h regenc.h regint.h regparse.h st.h
DEPS2 = $(patsubst %,$(IDIR2)/%,$(_DEPS2))

_DEPS3 = bindings_config.h
DEPS3 = $(patsubst %,$(IDIR3)/%,$(_DEPS3))

_DEPS4 = emscripten.h
DEPS4 = $(patsubst %,$(IDIR4)/%,$(_DEPS4))

_OBJ1 = ConvertUTF.o actions.o codetools.o common.o game.o mathops.o menu.o playlist.o text.o towlower.o variables.o callbacks.o coding.o errors.o locations.o memwatch.o objects.o statements.o time.o towupper.o variant.o
OBJ1 = $(patsubst %,$(ODIR1)/%,$(_OBJ1))

_OBJ2 = regcomp.o regenc.o regerror.o regexec.o regparse.o regsyntax.o regtrav.o regversion.o st.o
OBJ2 = $(patsubst %,$(ODIR2)/%,$(_OBJ2))

#_OBJ3 = hellomake.o hellofunc.o 
#OBJ3 = $(patsubst %,$(ODIR3)/%,$(_OBJ3))

_OBJ4 = emscripten_callbacks.o emscripten_coding.o emscripten_control.o
OBJ4 = $(patsubst %,$(ODIR4)/%,$(_OBJ4))

_OBJ5 = ascii.o cp1251.o koi8_r.o unicode.o utf16_be.o utf16_le.o utf32_be.o utf32_le.o
OBJ5 = $(patsubst %,$(ODIR5)/%,$(_OBJ5))

$(ODIR1)/%.o: $(IDIR1)/%.c $(DEPS1)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR2)/%.o: $(IDIR2)/%.c $(DEPS2)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR3)/%.o: $(IDIR3)/%.c $(DEPS3)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR4)/%.o: $(IDIR4)/%.c $(DEPS4)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR5)/%.o: $(IDIR5)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

qsp.js: qsp.bc
	$(CC) $^  -o $@ $(QCFLAGS)

qsp.bc: $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f qsp.bc qsp.js qsp.js.mem $(ODIR1)/*.o $(ODIR2)/*.o $(ODIR3)/*.o $(ODIR4)/*.o $(ODIR5)/*.o
