import sys
from struct import *
import serial
import serial.tools.list_ports
import threading
import binascii
import time
import re
import os
import os.path
from xlutils.copy import copy
import struct
from win32api import GetSystemMetrics
from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QApplication, QMainWindow, QMessageBox, QDialog, QInputDialog, QLineEdit, QListWidget
from PyQt5.QtCore import QCoreApplication, pyqtSignal, QObject
from PyQt5.QtGui import QIcon
from ZJUDancer import *
from PyQt5.QtWidgets import QFileDialog


class MainWindow(QMainWindow, Ui_MainWindow):
    ServoNum = 18
    PosData = [1985, 580, 1773, 1270, 2356, 2000, 1849, 1720,
               1500, 2329, 2488, 1857, 2018, 2200, 2323, 2144, 1890, 2345]
    IDList = [0x01, 0x08, 0x10, 0x12, 0x15, 0x16, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x09, 0x0A, 0x0B, 0x0C,
              0x0D, 0x0E]
    OrderBuffer = []
    pointer = 0
    LEN = 0
    sendFlag = 0
    ReceviceTime = 0.05
    Checksum = 0
    ser = None
    Order = []
    SendNum = 0
    Message = QtCore.pyqtSignal(str)

    Label_Data_RAW = QtCore.pyqtSignal(str)
    Label_Data_Rev = QtCore.pyqtSignal(str)
    Label_Speed = QtCore.pyqtSignal(str)
    Label_Pos = QtCore.pyqtSignal(str)
    Label_ADIS = QtCore.pyqtSignal(str)
    Label_Foot = QtCore.pyqtSignal(str)

    def __init__(self, parent=None):
        super(MainWindow, self).__init__(parent)
        self.setupUi(self)
        self.setWindowTitle('调试软件V1.0')
        self.showMaximized()
        self.PB_OpenSerial.clicked.connect(self.OpenPort)
        self.PB_FreshList.clicked.connect(self.RefreshPortList)
        self.PB_CloseSerial.clicked.connect(self.ClosePort)
        self.PB_ClearLog.clicked.connect(self.ClearLog)
        self.PB_SwitchLog.clicked.connect(self.SwitchLog)
        self.SerialLog.textChanged.connect(self.GotoBottom)

        # self.Message.connect(self.SerialLog.append)
        # self.Label_TempValue.connect(self.TempNow.setText)
        # self.Label_SerNum.connect(self.SerNum.setText)
        # self.Label_Status.connect(self.MechineStatusShow)
        # self.Label_WorkData.connect(self.WorkDataShow)
        # self.Label_CountData.connect(self.CountDataShow)
        self.Label_Data_RAW.connect(self.LB_SendBuffer.setText)
        self.Label_Data_Rev.connect(self.LB_RevRaw.setText)
        self.Label_Speed.connect(self.LB_Speed.setText)
        self.Label_Pos.connect(self.LB_Pos.setText)
        self.Label_ADIS.connect(self.LB_ADIS.setText)
        self.Label_Foot.connect(self.LB_FootData.setText)
        self.PBDisable()
        self.FootData_CTRL.setChecked(False)
        self.FootData_CTRL.stateChanged.connect(self.Foot_Change)

        self.RefreshPortList()
        self.RevCount = 0

    def SendData(self, Ins):
        self.Order = [0 for x in range(5)]
        self.Order[0] = 0xFF
        self.Order[1] = 0xFF
        self.Order[2] = 0xFD
        self.Order[3] = 0x00
        self.Order[4] = 0xFE
        Len = 7 + self.ServoNum * 3
        self.Order.append(Len & 0xFF)  # 修改
        self.Order.append((Len >> 8) & 0xFF)
        self.Order.append(Ins)
        self.Order.append(0x74)
        self.Order.append(0x00)
        self.Order.append(0x02)
        self.Order.append(0x00)
        for i in range(self.ServoNum):
            self.Order.append(self.IDList[i])
            self.Order.append(self.PosData[i] & 0xFF)
            self.Order.append((self.PosData[i] >> 8) & 0xFF)
        CRC = self.CRC_Acc(0, self.Order, 12 + 3 * self.ServoNum)
        self.Order.append(CRC & 0xFF)
        self.Order.append((CRC) >> 8 & 0xFF)
        self.Data_RAW("发送数据： " + re.sub(r"(?<=\w)(?=(?:\w\w)+$)",
                                        " ", bytes(self.Order).hex()).upper())
        self.ser.write(bytes(self.Order))

    def CRC_Acc(self, crc_accum, data_blk_ptr, data_blk_size):
        crc_table = [0x0000,
                     0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
                     0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027,
                     0x0022, 0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D,
                     0x8077, 0x0072, 0x0050, 0x8055, 0x805F, 0x005A, 0x804B,
                     0x004E, 0x0044, 0x8041, 0x80C3, 0x00C6, 0x00CC, 0x80C9,
                     0x00D8, 0x80DD, 0x80D7, 0x00D2, 0x00F0, 0x80F5, 0x80FF,
                     0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1, 0x00A0, 0x80A5,
                     0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1, 0x8093,
                     0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
                     0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197,
                     0x0192, 0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE,
                     0x01A4, 0x81A1, 0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB,
                     0x01FE, 0x01F4, 0x81F1, 0x81D3, 0x01D6, 0x01DC, 0x81D9,
                     0x01C8, 0x81CD, 0x81C7, 0x01C2, 0x0140, 0x8145, 0x814F,
                     0x014A, 0x815B, 0x015E, 0x0154, 0x8151, 0x8173, 0x0176,
                     0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162, 0x8123,
                     0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
                     0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104,
                     0x8101, 0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D,
                     0x8317, 0x0312, 0x0330, 0x8335, 0x833F, 0x033A, 0x832B,
                     0x032E, 0x0324, 0x8321, 0x0360, 0x8365, 0x836F, 0x036A,
                     0x837B, 0x037E, 0x0374, 0x8371, 0x8353, 0x0356, 0x035C,
                     0x8359, 0x0348, 0x834D, 0x8347, 0x0342, 0x03C0, 0x83C5,
                     0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1, 0x83F3,
                     0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
                     0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7,
                     0x03B2, 0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E,
                     0x0384, 0x8381, 0x0280, 0x8285, 0x828F, 0x028A, 0x829B,
                     0x029E, 0x0294, 0x8291, 0x82B3, 0x02B6, 0x02BC, 0x82B9,
                     0x02A8, 0x82AD, 0x82A7, 0x02A2, 0x82E3, 0x02E6, 0x02EC,
                     0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2, 0x02D0, 0x82D5,
                     0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1, 0x8243,
                     0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,
                     0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264,
                     0x8261, 0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E,
                     0x0234, 0x8231, 0x8213, 0x0216, 0x021C, 0x8219, 0x0208,
                     0x820D, 0x8207, 0x0202]

        for j in range(0, data_blk_size):
            i = ((crc_accum >> 8) ^ data_blk_ptr[j]) & 0xFF
            crc_accum = ((crc_accum << 8) ^ crc_table[i]) & 0xFFFF
        return crc_accum

    def SendTestOrder(self):
        while (self.ser.isOpen() and self.SendData_CTRL.isChecked()):  # 数据发送
            self.sendFlag = 1
            self.SendData(0x83)
            self.sendFlag = 0
            time.sleep(1)

    def Foot_Change(self):
        if self.FootData_CTRL.isChecked() == 1:
            while (self.sendFlag != 1):
                pass
            self.SendData(0x84)  # 发送开启足底命令
        else:
            while (self.sendFlag != 1):
                pass
            self.SendData(0x85)  # 发送结束足底命令

    def PBDisable(self):
        self.PB_CloseSerial.setDisabled(True)
        self.PB_ClearLog.setDisabled(True)
        self.PB_SwitchLog.setDisabled(True)
        self.PB_OpenSerial.setEnabled(True)

    def PBEnable(self):
        self.PB_CloseSerial.setEnabled(True)
        self.PB_ClearLog.setEnabled(True)
        self.PB_SwitchLog.setEnabled(True)
        self.PB_OpenSerial.setDisabled(True)

    def ClearLog(self):
        self.SerialLog.clear()

    def SwitchLog(self):
        if self.PB_SwitchLog.text() == "暂停显示":
            self.Message.disconnect(self.PrintInSerialLog)
            self.PB_SwitchLog.setText("开启显示")
        else:
            self.Message.connect(self.PrintInSerialLog)
            self.PB_SwitchLog.setText("暂停显示")

    def OpenPort(self):
        self.ser = serial.Serial()
        self.ser.port = self.CB_SerialList.currentText()
        self.ser.baudrate = 1000000
        self.ser.bytesize = 8
        self.ser.parity = 'N'
        self.ser.stopbits = 1
        try:
            self.ser.open()
        except serial.SerialException:
            self.SerialLog.append("LOG：打开失败,刷新列表再试")
            return
        if (self.ser.isOpen()):
            self.SerialLog.append("LOG：打开成功")
            self.PBEnable()
            self.t1 = threading.Thread(target=self.ReceiveData)
            self.t1.setDaemon(True)
            self.t1.start()
            self.t2 = threading.Thread(target=self.PrintSecond)
            self.t2.setDaemon(True)
            self.t2.start()
            self.t3 = threading.Thread(target=self.SendTestOrder)
            self.t3.setDaemon(True)
            self.t3.start()
        else:
            self.SerialLog.append("LOG：打开失败")

    def PrintSecond(self):
        while True:
            time.sleep(1.0)
            print("Time: %d" % self.RevCount)

    def ClosePort(self):
        self.ser.close()
        self.PBDisable()

    def RefreshPortList(self):
        plist = list(serial.tools.list_ports.comports())
        if len(plist) <= 0:
            self.CB_SerialList.clear()
            self.CB_SerialList.addItem(str("无可用串口"))
        else:
            self.PB_OpenSerial.setEnabled(True)
            self.CB_SerialList.clear()
            for i in range(0, len(plist)):
                plist_0 = list(plist[i])
                self.SerialLog.append(str(plist_0[1]))
                self.CB_SerialList.addItem(str(plist_0[0]))
                self.CB_SerialList.setCurrentIndex(1)

    def ReceiveData(self):
        while (self.ser.isOpen()):
            try:
                time.sleep(self.ReceviceTime)
                size = self.ser.inWaiting()
            except:
                if (self.ser.isOpen()):
                    self.ClosePort()
                    self.ser = None
                    self.SerialLog.append("LOG：串口丢失")
                else:
                    self.ClosePort()
                    self.ser = None
                    self.SerialLog.append("LOG：串口停止")
                self.PB_OpenSerial.setEnabled(True)
                return None
            if size:
                res_data = self.ser.read(size)
                self.InfoAnalysis(res_data)
            self.ser.reset_input_buffer()

    def InfoAnalysis(self, data):  # 修改
        res_data = bytearray(data)
        while len(res_data) != 0:
            temp = res_data.pop(0)
            if self.pointer < 2:
                self.InfoLoad(temp)
                continue
            if self.pointer == 2:
                if (self.OrderBuffer[0] == 0x12) and (self.OrderBuffer[1] == 0x34):
                    self.InfoLoad(temp)
                    self.LEN = temp
                else:
                    self.OrderBuffer[0] = self.OrderBuffer[1]
                    self.OrderBuffer[1] = temp
                continue
            if self.pointer > 2:
                if self.pointer < self.LEN:
                    self.InfoLoad(temp)
                    if self.pointer == self.LEN:
                        if (self.OrderBuffer[self.LEN - 1] == 0x2F) and (self.OrderBuffer[self.LEN - 2] == 0x1F):
                            for i in range(2, self.LEN - 3):
                                self.Checksum = self.Checksum + \
                                    self.OrderBuffer[i]
                            if self.OrderBuffer[-3] == self.Checksum & 0xFF:
                                self.RevCount += 1
                                self.OrderAnalysis(self.OrderBuffer)
                            else:
                                self.DisPlayMessage(
                                    "LOG：校验错误,校验值应当为: %X" % (self.Checksum & 0xFF))
                                # 发送重发指令
                        else:
                            self.DisPlayMessage(
                                "LOG：指令尾错误 %X %X" % (self.OrderBuffer[self.LEN - 2], self.OrderBuffer[self.LEN - 1]))
                        self.Checksum = 0
                        self.pointer = 0
                        self.OrderBuffer = []
                    else:
                        continue
            # if self.pointer < 4:
            #     self.InfoLoad(temp)
            #     continue
            # if self.pointer == 4:
            #     if (self.OrderBuffer[0] == 0xFF) and (self.OrderBuffer[1] == 0xFF)and (self.OrderBuffer[2] == 0xFD)and (self.OrderBuffer[3] == 0x00):
            #         self.InfoLoad(temp)
            #         self.LEN = temp
            #     else:
            #         self.OrderBuffer[0] = self.OrderBuffer[1]
            #         self.OrderBuffer[1] = self.OrderBuffer[2]
            #         self.OrderBuffer[2] = self.OrderBuffer[3]
            #         self.OrderBuffer[3] = temp
            #     continue
            # if self.pointer <= 6:
            #     self.InfoLoad(temp)
            #     if self.pointer==7:
            #         self.LEN=((self.OrderBuffer[6]<<8)|self.OrderBuffer[5])+7
            #     continue
            # if self.pointer > 6:
            #     if self.pointer < self.LEN:
            #         self.InfoLoad(temp)
            #         if self.pointer == self.LEN:
            #             if (self.CRC_Acc(0,self.OrderBuffer,self.LEN-2)==(self.OrderBuffer[self.LEN-1]<<8)|self.OrderBuffer[self.LEN-2]):
            #                 self.OrderAnalysis(self.OrderBuffer)
            #             else:
            #                 self.DisPlayMessage("校验失败")
            #             self.Checksum = 0
            #             self.pointer = 0
            #             self.OrderBuffer = []
            #         else:
            #             continue

    def OrderAnalysis(self, Data):
        self.Data_Rev("原始数据： " + re.sub(r"(?<=\w)(?=(?:\w\w)+$)",
                                        " ", bytes(Data).hex()).upper())
        self.Data_DisPlay(Data)

    def Data_DisPlay(self, Data):
        Res = []
        str1 = "位置："
        str2 = "速度："
        str3 = "陀螺仪数据："
        str4 = "足底板数据："
        for i in range(self.ServoNum):
            Res.append(Data[3 + 4 * i] << 8 | Data[4 + 4 * i])
            str1 = str1 + " {:10d}".format(Res[-1])
        for i in range(self.ServoNum):
            Res.append(Data[5 + 4 * i] << 8 | Data[6 + 4 * i])
            str2 = str2 + " {:10d}".format(Res[-1])
        if len(Data) >= 38+4*self.ServoNum:
            for i in range(9):
                Res.append((Data[19+4 * self.ServoNum + 2 * i] << 8)
                           | Data[20 + 4 * self.ServoNum + 2 * i])
                str3 = str3 + " {:10d}".format(Res[-1])

            for i in range(8):
                Temp = (Data[3 + 4 * self.ServoNum + 2 * i]<< 8) | Data[4 + 4 * self.ServoNum + 2 * i]
                if Temp >= 32768:
                    Temp = Temp - 65535

                Res.append(Temp)
                str4 = str4 + " {:10d}".format(Res[-1])
        for i in range(8):
            Temp = (Data[3 + 4 * self.ServoNum + 2 * i] <<8) | Data[4 + 4 * self.ServoNum + 2 * i]
            if Temp >= 32768:
                Temp = Temp - 65535
            Res.append(Temp)
        str4 = str4 + " {:10d}".format(Res[-1])
        self.Label_Speed.emit(str1)
        self.Label_Pos.emit(str2)
        self.Label_ADIS.emit(str3)
        self.Label_Foot.emit(str4)

    def Data_Rev(self, str):
        self.Label_Data_Rev.emit(str)

    def Data_RAW(self, str):
        self.Label_Data_RAW.emit(str)

    def DisPlayMessage(self, str):
        self.Message.emit(str)

    def Bytes_Printf(self, data):
        str = re.sub(r"(?<=\w)(?=(?:\w\w)+$)", " ", bytes(data).hex()).upper()
        return str

    def LB_SerNum(self, str):
        self.Label_SerNum.emit(str)

    def LB_Temp(self, str):
        self.Label_TempValue.emit(str)

    def InfoLoad(self, data):
        self.OrderBuffer.append(data)
        self.pointer = self.pointer + 1

    def GotoBottom(self):
        cursor = self.SerialLog.textCursor()
        pos = len(self.SerialLog.toPlainText())
        cursor.setPosition(pos - 1)
        self.SerialLog.setTextCursor(cursor)


if __name__ == "__main__":
    app = QApplication(sys.argv)
    mywin = MainWindow()
    mywin.show()
    sys.exit(app.exec_())
