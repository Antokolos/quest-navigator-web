/*
*  Bridge:
*           Javascript -> QSP JS compiled with Emscripten
*
*/

var qspLibMode = "PHONEGAP";       // "AIR", "PHONEGAP" - устанавливаем для того, 
                                   // чтобы api.js мог выполнять различный код в зависимости от платформы

var QspLib = {
    
    registerJsCallback: function(callbackFunction, callbackName)
    {
    // ???
    // QspSetCallBack ????
        //return cordova.exec(callbackFunction, null, "QspLib", "registerJsCallback", [callbackName]);
    },

    initLib: function(onInited) {
        return Module.cwrap('QSPInit', null)(); 
        //return cordova.exec(onInited, null, "QspLib", "initLib", []);
    },
    
    restartGame: function() {
        return Module.cwrap('QSPRestartGame', null, [true])();
        //return cordova.exec(null, null, "QspLib", "restartGame", []);
    },
    
    version: function(types, success, fail) {
        var s = Module.cwrap('QSPGetVersionW', 'string', [])()
        console.log({ss: s, ss0: s[0], ss1: s[1], ss2: s[2]});
        return s;
      //return cordova.exec(success, fail, "QspLib", "version", types);
    },
    
    selectAction: function(index) {
        return Module.cwrap('QSPSetSelActionIndex', 'boolean', [index, true])();
        //return cordova.exec(null, null, "QspLib", "selectAction", [index]);
    },

    executeAction: function(index) {
        return Module.cwrap('QSPExecuteSelActionCode', 'boolean', [true])();
        //return cordova.exec(null, null, "QspLib", "executeAction", [index]);
    },
    
    selectObject: function(index) {
        return Module.cwrap('QSPSetSelObjectIndex', 'boolean', [index, true])();
        //return cordova.exec(null, null, "QspLib", "selectObject", [index]);
    },
    
    loadGame: function() {
    // ???
        return Module.cwrap('QSPLoadGameWorld', 'boolean', [])();
        //return cordova.exec(null, null, "QspLib", "loadGame", []);
    },
    
    saveGame: function() {
        return Module.cwrap('QSPSaveGame', 'boolean', [])();
        //return cordova.exec(null, null, "QspLib", "saveGame", []);
    },
    
    saveSlotSelected: function(index, open) {
    // ???
        //var mode = open ? 1 : 0;
        //return cordova.exec(null, null, "QspLib", "saveSlotSelected", [index, mode]);
    },

    msgResult: function() {
    // ???
//        return cordova.exec(null, null, "QspLib", "msgResult", []);
    },
    
    errorResult: function() {
    // ???
//        return cordova.exec(null, null, "QspLib", "errorResult", []);
    },
    
    userMenuResult: function(index) {
    // ???
//        return cordova.exec(null, null, "QspLib", "userMenuResult", [index]);
    },
    
    inputResult: function(text) {
    // ???
//<------>QSP_EXTERN QSP_BOOL QSPExecUserInput(QSP_BOOL isRefresh);^M
        return Module.cwrap('QSPSetInputStrText', null, [text])();
        //return cordova.exec(null, null, "QspLib", "inputResult", [text]);
    },

    setMute: function(mute) {
    // ???
//        return cordova.exec(null, null, "QspLib", "setMute", [mute]);
    },
	
	// Для того, чтобы приложение на Андроиде не закрывать по кнопке BACK, а отправлять в фоновый режим
	moveTaskToBackground: function() {
	// ???
		//return cordova.exec(null, null, "QspLib", "moveTaskToBackground", []);
	}
};


//Функция для предзагрузки картинок (сейчас не используется)
jQuery.preloadImages = function () {
    if (typeof arguments[arguments.length - 1] == 'function') {
        var callback = arguments[arguments.length - 1];
    } else {
        var callback = false;
    }
    if (typeof arguments[0] == 'object') {
        var images = arguments[0];
        var n = images.length;
    } else {
        var images = arguments;
        var n = images.length - 1;
    }
    if (n == 0 && typeof callback == 'function') {
        callback();
        return;
    }
    var not_loaded = n;
    for (var i = 0; i < n; i++) {
    	$(images[i]).imagesLoaded().always(function() {
                                                        if (--not_loaded < 1 && typeof callback == 'function') {
                                                            callback();
                                                        }
                                                        });
    }
}

var qspInitLevel = 0;

function qspInitNext()
{
    setTimeout(function() {
        qspInitLevel++;
        if (qspInitLevel == 1)
            QspLib.initLib(qspInitNext);
        else if (qspInitLevel == 2)
            QspLib.registerJsCallback(qspSetGroupedContent,		"qspSetGroupedContent");    //QspLib4
        else if (qspInitLevel == 3)
            QspLib.registerJsCallback(qspMsg,					"qspMsg");                  //QspLib5
        else if (qspInitLevel == 4)
            QspLib.registerJsCallback(qspView,                	"qspView");                 //QspLib6
        else if (qspInitLevel == 5)
            QspLib.registerJsCallback(qspInput,               	"qspInput");                //QspLib7
        else if (qspInitLevel == 6)
            QspLib.registerJsCallback(qspMenu,                	"qspMenu");                 //QspLib8
        else if (qspInitLevel == 7)
            QspLib.registerJsCallback(qspError,               	"qspError");                //QspLib9
        else if (qspInitLevel == 8)
            QspLib.registerJsCallback(qspShowSaveSlotsDialog,	"qspShowSaveSlotsDialog");  //QspLib10
        else if (qspInitLevel == 9)
            QspLib.restartGame();
        else
        {
            // no more init
        }
    }, 0);
}

function onPhoneGapDeviceReady() {
    // Now safe to use the PhoneGap API
	qspIsAndroid = device.platform === "Android";
	qspIsIos = (device.platform === "iPhone") || (device.platform === "iPad");
	if (qspIsAndroid)
	{
		// Переопределяем поведение кнопки BACK
		document.addEventListener("backbutton", qspBackKeyPressed, false);
		// По кнопке MENU вызываем диалог системного меню
		document.addEventListener("menubutton", qspShowSystemMenu, false);
	}

	// Сообщаем API, что нам стал известен тип устройства.
	qspSetDevice();

	qspInitNext();
}

function qspLibOnInitApi() {
	document.addEventListener("deviceready", onPhoneGapDeviceReady, false);
}