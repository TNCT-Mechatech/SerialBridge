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
* 最後にmake allをして静的ライブラリを生成します
1. SerialBridgeディレクトリに移動します
2. binディレクトリを作成します
```shell
mkdir bin
```
3. 静的ライブラリを生成します
```shell
make all
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
TODO

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
