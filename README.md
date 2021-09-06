# SerialBridge
##  概要
本ライブラリは、PC(ROS)/Arduino/Mbed間でシリアル通信を行うためのライブラリです。
デバイス間での数値のやりとりを可能にします。  
本ライブラリはマルチプラットフォームに対応するためにC++で書かれています。  

## 導入
### Linux/ROS
* プロジェクトにgitを利用している場合  
submoduleを使います  
1. プロジェクトディレクトリに移動します
2. terminalで以下のコマンドを実行しsubmoduleとしてクローンします  
```shell
git submodule add https://github.com/TNCT-Mechatech/SerialBridge SerialBridge
echo 'commit'
git add SerialBridge
git commit -m 'Add SerialBridge as a module'
```  
* プロジェクトにgitを利用していない場合
1. プロジェクトディレクトリに移動します
2. terminalで以下のコマンドを実行しレポジトリをクローンします
```
git clone https://github.com/TNCT-Mechatech/SerialBridge 
```  
* 最後にmake allをして静的ライブラリを生成して、リンクさせます
1. SerialBridgeディレクトリに移動します
2. binディレクトリを作成します
```shell
mkdir bin
```
3. 静的ライブラリを生成します
```shell
make all
```  
4. CMakeLists.txtに以下のコードを追加
```cmake
target_link_libraries(YOUR_PROJECT_NAME ${CMAKE_SOURCE_DIR}/SerialBridge/bin/libSerialBridge.a)
```  

### Arduino
1. librariesに移動します
2. terminalで以下のコマンドを実行しレポジトリをクローンします
```
git clone https://github.com/TNCT-Mechatech/SerialBridge 
```  

### Mbed
**[Online Compiler](https://ide.mbed.com/)での開発のみ動作確認されています**  
1. 任意のプロジェクトを作成します。Mbed-os 2,5,6で動作します
2. レポジトリをプログラムにクローンします  
[任意のプロジェクトを右クリック] -> [ライブラリのインポート] -> [URLからインポート] -> 
[Source URLに[レポジトリのURL](https://github.com/TNCT-Mechatech/SerialBridge/)を入力] -> [Import AsをLibrary] -> [Import]  
※必要に応じてリビジョンから任意のコミットをマージしてください  

## 使用方法
### 初期化
#### **Linux/ROS**
```c++
#include <SerialBridge.hpp>
//  serial driver for linux/ros
#include <LinuxHardwareSerial.hpp>

#define SERIAL_PATH "/dev/ttyUSB0"

SerialDev *dev = new LinuxHardwareSerial(SERIAL_PATH, B9600);
SerialBridge serial(dev);
```  
SerialパスはLinuxのシンボリックリンクを使用できます。あらかじめボードを接続する場所を決めて接続します。  
以下のような形でボードがどのUSBポートに接続されているか確認できます。  
どのポートにするかを決めてLinuxHardwareSerialに渡してください。
```shell
$ ls /dev/serial/by-path/
pci-0000:00:1a.0-usb-0:1:1.0-port0 //右側USBポート
pci-0000:00:1d.0-usb-0:2:1.0-port0 //左側手前
pci-0000:00:1d.0-usb-0:1:1.0-port0 //左側奥
pci-0000:00:1d.7-usb-0:1.4:1.0-port0 //左側奥＋USBハブ
pci-0000:00:1d.7-usb-0:1.4.1:1.0-port0 //左側奥＋USBハブ＋USBハブ
```  
ex) 右側USBポートを使用する場合  
```c++
#define SERIAL_PATH "/dev/serial/by-path/pci-0000:00:1a.0-usb-0:1:1.0-port0"
```  
周波数は変更可能です  

#### **Arduino**
```c++
#include <SerialBridge.hpp>
//  serial driver for arduino
#include <InoHardwareSerial.hpp>

//  Pass the serial derived class entity for Arduino to SerialDev.
SerialDev *dev = new InoHardwareSerial(&Serial);
SerialBridge serial(dev);

void setup()
{
    Serial.begin(9600);
}

```  
周波数は変更可能です  

#### **Mbed**
* Mbed-os2(Nucleo F303K8, etc.)の場合  
```c++
#include <SerialBridge.hpp>
#include <MbedHardwareSerial.hpp>

SerialDev *dev = new MbedHardwareSerial(new Serial(USBTX, USBRX, 9600));
SerialBridge serial(dev);
```  
周波数は変更可能です  
* Mbed-os5,6の場合  
```c++
#include <SerialBridge.hpp>
#include <MbedHardwareSerial.hpp>

SerialDev *dev = new MbedHardwareSerial(new BufferedSerial(USBTX, USBRX, 9600));
SerialBridge serial(dev);
```  
周波数は変更可能です  

### Messaseの用意とフレームの追加
1. Messageを用意する　　
structを使ってMessageの内容を決めます  
```c++
typedef struct Vector3
{
    float x;
    float y;
    float z;
} vector3_t;
```  
このようなstructをメッセージとして使ってみましょう  
Messageを作成するには[Message.hpp](src/Message.hpp)をインクルードする必要があります  
Vector3.h
```c++
#ifndef VECTOR3_H_
#define VECTOR3_H_

#include <Message.hpp>

typedef struct Vector3
{
    float x;
    float y;
    float z;
} vector3_t;

//  create message
typedef sb::Message<vector3_t> Vector3;

#endif
```  
2. SerialBridgeにMessageを登録  
作成したMessageを利用するには、フレームとして登録する必要があります  
SerialBridge::add_frame(id, msg)を使います。
引数として、id(任意の値)とMessageのポインターを渡す必要があります。  
idは重複しないようにしてください。  
```c++
#include <SerialBridge.hpp>
//  your message header
#include <Vector3.h>

SerialBridge serial(...);

//  Message
Vector3 msg0;

void main(){
    serial.add_frame(0, &msg0);
}
```
この場合はmsg0をid 0番と登録したことになります。  

### 通信してみる
#### **送信**
関数  
- SerialBridge::write(id)  
idはadd_frame(id, msg)で追加したものを使用します  
Vector3に値を入れて送信してみましょう  
```c++
#include <SerialBridge.hpp>
//  your message header
#include <Vector3.h>

SerialBridge serial(...);

//  Message
Vector3 msg0;   //  sender

void main(){
    serial.add_frame(0, &msg0);

    msg0.data.x = 0.123;
    msg0.data.y = 0.456;
    msg0.data.z = 0.789;
}

void loop(){
    //  write
    serial.write(0);
}
```
Messageの値には、Message.data.VARIABLE_NAME でアクセスできます  
送信には、serial.write(id)をつかいます  

#### **受信**
関数
- SerialBridge::update()  
受信したパケットをデコードします。read()関数を使用するたびに呼び出してください。  
- SerialBridge::read()  
メッセージを読み込みます。受信に成功すると0を返します。  
Vector3を受信してみましょう  
```c++
#include <SerialBridge.hpp>
//  your message header
#include <Vector3.h>

SerialBridge serial(...);

//  Message
Vector3 msg1;   //  listener

void main(){
    serial.add_frame(0, &msg1);
}

void loop(){
    //  update and read
    serial.update()
    if (serial.read() == 0)
    {
        printf("%f %f %f \n\r", msg1.data.x, mag1.data.y, msg1.data.z);
    }
}
```  


## 開発環境
- Ubuntu 20.04 LTS  
- C++ 11

## Commit Prefix
|Prefix   |内容     |
|---------|--------|
|[Add]    |ファイル追加 / 機能追加|
|[Delete] | ファイル削除 / 機能削除|
|[Update] | 機能修正 (バグ修正を除く)|
|[Fix]    |バグ修正|
|[HotFix] |クリティカルなバグ修正|
|[Clean]  |リファクタリング / コード整理|
|[Change] | 仕様変更|
|[Rename] | 名前変更|
|[Docs] | ドキュメント(説明)系の編集|
|[Debug] | デバッグコードに関する編集 |  

## License
Apache-2.0 License

## Developer
- [TaiyouKomazawa](https://github.com/TaiyouKomazawa)(Outside Contributor)
- [testusuke](https://github.com/testusuke)
