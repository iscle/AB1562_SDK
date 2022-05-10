# ==============================================================================
# add all .S and .c files in sub-folder of app_src/ into target source
# ==============================================================================
ASM_SRC		+=	\
$(addprefix src/, \
STARTUP.S \
)

C_SRC		+=	\
$(addprefix src/, \
main.c \
sys_init.c \
syscalls.c \
regions_init.c \
dcxo_capid.c\
)


C_SRC		+=	\
$(addprefix src/APP/, \
App.c \
App_ResourceManager.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_State/, \
App_State.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_System/, \
App_System.c \
App_SystemNvKeyHandler.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_System/APP_Key/, \
App_KeyManage.c \
key.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_System/APP_Pairing/, \
App_Pairing.c \
Pairing_NvkeyHandler.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_System/APP_Power/, \
App_PowerOff.c \
App_PowerOn.c \
Power_NvkeyHandler.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_System/APP_RaceCmd/, \
App_RaceCmd.c \
App_RaceCmd_Connection.c \
App_RaceCmd_KeyCode.c \
App_RaceCmd_Fcd.c \
App_RaceCmd_Ama.c \
App_RaceCmd_Fota.c \
App_RaceCmd_System.c \
App_RaceCmd_Anc.c \
App_RaceCmd_AirDump.c \
App_RaceCmd_RealTime.c \
app_racecmd_hostaudio.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_System/APP_EventKey/, \
App_AvrcpKey.c \
App_EventKeyHandler.c \
App_HFPKey.c \
App_Mp3Key.c \
App_Nvkey_KeymapHandler.c \
App_SystemKey.c \
App_VpKey.c \
App_CheckKeyValid.c \
App_TestmodeKey.c \
App_AEKey.c \
App_AMAKey.c \
App_XiaoaiKey.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_DspControl/, \
AudioDSP_AncPassThru/AudioDSP_AncPassThru.c \
AudioDSP_Vp/AudioDSP_Vp.c \
AudioDSP_Registration.c \
AudioDSP_StreamManager.c \
AudioDSP_VoiceCommand/App_Vad.c\
AudioDSP_FeatureManager.c \
APP_Encoder.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_Hfp/, \
APP_HfpScoHandler.c \
APP_HfpIncomingCallMediaHandler.c \
APP_HfpNvkeyHandler.c \
App_Hfp.c \
APP_HfpState.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_NVRAM/APP_Nvram_Type/, \
App_Nvram_Type.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_NVRAM/APP_Nvram_DriverVariation/, \
App_Nvram_DriverVariation.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_Connection/, \
App_Connection.c \
App_ReConnection.c \
App_LinkKey.c \
App_Connection_NVKeyHandler.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_Customer/, \
App_CustomerTimer.c \
APP_CustApp_LE.c\
)

C_SRC		+=	\
$(addprefix src/APP/APP_A2dp/, \
App_A2dp_NVKeyHandler.c \
App_A2dp.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_System/APP_VolumeControl/, \
App_VolumeManage.c \
App_VolumeProcess.c \
App_VolumeSetting.c \
App_VolumeScoMicProcess.c \
App_VolumeLineinProcess.c \
App_VolumeScoProcess.c \
App_VolumeA2dpProcess.c \
App_VolumeMp3Process.c \
App_VolumeVpRtProcess.c \
App_VolumeAiroThruProcess.c \
Volume_NvkeyHandler.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_System/APP_ChargeBattery/, \
App_ChargeBattery.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_LedControl/, \
App_LedControl.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_System/APP_Media/, \
App_Media.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_Avrcp/, \
App_Avrcp.c \
App_Avrcp_NVKeyHandler.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_LineIn/, \
App_LineIn.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_AudioDriver/, \
APP_AudioDriver.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_TestMode/, \
App_TestMode.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_DspMusicEffect/, \
App_Peq.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_Sniff/, \
App_SniffHandler.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_Nvkey/, \
App_Nvkey.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_PEQ/, \
App_PeqManage.c \
App_PeqProcess.c \
App_PeqMount.c \
Peq_NvkeyHandler.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_AirApp/, \
APP_AirApp.c \
APP_CustApp.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_AiroThru/, \
APP_AiroThru.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_Qos/, \
APP_Qos.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_FCD/, \
App_Fcd.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_Ama/, \
App_Ama.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_Ama/NVKey/, \
Ama_NvkeyHandler.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_Xiaoai/, \
App_Xiaoai.c \
App_XiaoaiAdv.c \
App_Xiaoai_VendorHandler.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_Ntc/, \
App_Ntc.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_GattServer/, \
App_GattServer.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_Gfp/, \
App_Gfp.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_Ble/, \
App_Ble.c \
)

C_SRC		+=	\
$(addprefix src/APP/APP_Swift/, \
App_Swift.c \
)

#ASM_SRC		+=	\
#$(addprefix src/, \
#)



