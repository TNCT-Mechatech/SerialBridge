# SerialBridge

## 概要

本ライブラリは、PC,MCU(Arduino,Mbed,STM32)間でUART,CAN FDによるシリアル通信を行うためのライブラリです。   
用途に合わせて、簡単にパケットデータを割り当てることができる仕組みをとっています。  
本ライブラリは、クロスプラットフォーム化のために非STL依存のC++及びC言語によって記述されています。

## 導入

### Linux

* プロジェクトでgit管理を既に用いている場合     
  submoduleとして利用します
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
    2. makeコマンド(下記)により、静的ライブラリを生成します
    ```shell
    make all
    ```
    3. ビルド時

    * CMakeを用いる場合は、CMakeLists.txtに以下のコードを追加
    ```cmake
    target_link_libraries(YOUR_PROJECT_NAME ${CMAKE_SOURCE_DIR}/SerialBridge/bin/libSerialBridge.a)
    ```
    * gccから直接ビルドする場合は、ターゲットと同じディレクトリ内で次のようにコマンドを実行
    ```shell
    g++ (target file).cpp (your directory path)/SerialBridge/bin/libSerialBridge.a \
     -I (your directory path)/SerialBridge/src
    ```
  `(target file)`: ビルドターゲットのファイル名   
  `(your directory path)`: 本ライブラリをクローンしたディレクトリパス

### Arduino

1. librariesに移動します
2. terminalで以下のコマンドを実行しレポジトリをクローンします

```
git clone https://github.com/TNCT-Mechatech/SerialBridge
```

### Mbed

1. 任意のプロジェクトを作成します
2. レポジトリをプログラムにクローンします
    1. 任意のプロジェクトを右クリック
    2. Add Mbed Library
    3. URLに[レポジトリのURL](https://github.com/TNCT-Mechatech/SerialBridge/)を入力し、Next
    4. 特に理由がなければ、`main` を選ぶ
3. `mbed_app.json`ファイルを作成し、以下の内容を入力

```json
{
  "config": {
    "serialbridge_for_mbed": {
      "help": "Macro",
      "macro_name": "MBED",
      "value": 0
    }
  }
}
```

## SerialBridge with CAN FD

CAN FDを使う場合は、`SB_CANFD`マクロを宣言してください。

### Mbed

Mbedを使う場合は、`mbed_app.json`に追加してください。

```json
{
  "config": {
    "serialbridge_with_canfd": {
      "help": "Macro",
      "macro_name": "SB_CANFD",
      "value": 0
    }
  }
}
```

## 使用方法

### 初期化

初期化手順のみが各環境に依存します。    
`SerialDev`に渡されるシリアルデバイスのボーレートはデバイス間で同じ速度である必要があります。   
またフォーマットは、8データビット、ノンパリティが推奨されます。

#### **Linux**

```c++
#include <SerialBridge.hpp>
//  serial driver for linux/ros
#include <LinuxHardwareSerial.hpp>

SerialDev *dev = new LinuxHardwareSerial("/dev/ttyUSB0", B9600);
SerialBridge serial(dev);
```

Serialパスはデバイス接続時に自動生成されるシンボリックリンクも使用できます。    
このリンクはUSBの接続位置によって固有になるので、複数の同じデバイスをつないでも識別ができるようになります。  
以下に示す例のように、ボードがどのUSBポートに接続されているか確認できます。

```shell
$ ls /dev/serial/by-path/
pci-0000:00:1a.0-usb-0:1:1.0-port0 //右側USBポート
pci-0000:00:1d.0-usb-0:2:1.0-port0 //左側手前
pci-0000:00:1d.0-usb-0:1:1.0-port0 //左側奥
pci-0000:00:1d.7-usb-0:1.4:1.0-port0 //左側奥＋USBハブ
pci-0000:00:1d.7-usb-0:1.4.1:1.0-port0 //左側奥＋USBハブ＋USBハブ
```

とはいえ可視性が悪いので１個ずつシリアルデバイスを接続して確かめることをおすすめします。    
利用する際はマクロ定義で置き換えると引数部分がすっきりする。    
ex) 右側USBポートを使用する場合

```c++
#define SERIAL_PATH "/dev/serial/by-path/pci-0000:00:1a.0-usb-0:1:1.0-port0"
SerialDev *dev = new LinuxHardwareSerial(SERIAL_PATH, B9600);
```

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

#### **Mbed**

* Mbed-os2(Nucleo F303K8, etc.)の場合

```c++
#include <SerialBridge.hpp>
#include <MbedHardwareSerial.hpp>

SerialDev *dev = new MbedHardwareSerial(new Serial(USBTX, USBRX, 9600));
SerialBridge serial(dev);
```

* Mbed-os5,6の場合

```c++
#include <SerialBridge.hpp>
#include <MbedHardwareSerial.hpp>

SerialDev *dev = new MbedHardwareSerial(new BufferedSerial(USBTX, USBRX, 9600));
SerialBridge serial(dev);
```

### Messaseの用意とフレームの追加

1. Messageを用意する    
   structを使ってMessageの内容を決めます

```c++
typedef struct Vector3Type
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

typedef struct Vector3Type
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
Vector3 msg;

void main(){
    serial.add_frame(0, &msg);
}
```

この場合は`msg`をid 0番と登録したことになります。

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
Vector3 msg;   //  sender

void main(){
    serial.add_frame(0, &msg);

    msg.data.x = 0.123;
    msg.data.y = 0.456;
    msg.data.z = 0.789;
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

受信したパケットをデコードします。

- Message::was_update()

メッセージ内容が更新されたかどうかを確認できます。

```c++
#include <SerialBridge.hpp>
//  your message header
#include <Vector3.h>

SerialBridge serial(...);

//  Message
Vector3 msg;   //  listener

void main(){
    serial.add_frame(0, &msg);
}

void loop(){
    //  update and read
    serial.update()
    if (msg.was_updated())
    {
        printf("%f %f %f \n\r", msg.data.x, mag.data.y, msg.data.z);
    }
}
```

## 開発環境

- Ubuntu 20.04 LTS
- C++11 (GNU GCC 10.3.0)
- Arduino IDE 1.8.13
- Mbed OS 6.13.0 based

## Commit Prefix

- **feat**: 新機能
- **change**: 修正・削除
- **fix**: バグフィックス
- **docs**: ドキュメントに関する変更
- **style**: フォーマット等の変更
- **refactor**: リファクタに関する変更
- **debug**: デバック用のコード
- **test**: テストコードの追加・更新
- **chore**: GitHub Actions等タスクに関する変更

## License

Apache-2.0 License

## Developer

- [TaiyouKomazawa](https://github.com/TaiyouKomazawa)(Project Author)
- [testusuke](https://github.com/testusuke)
