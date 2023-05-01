# Linear Actuator Controller

これは"いしかわきょーすけ"さん作のM5Stack ATOM用直動機構を動かすための非公式ライブラリです。


---

## Project Status

<div style="display: flex">
    <div style="width:1em; background-color: red;margin-right:1em;"></div>
    <span style="">This project will not be refurbished unless requested.</span>
</div>


## Description


### parts list

| No. |          | Device type. | Device    |
| --- | -------- | ------------ | --------- |
| 1.  | Required | Board        | ATOM Lite |
| 2.  | Required | Board        | ATOM Lite |


## Installation

This system is compiled using PlatformIO. Please install the extension from VS Code's extension recommendations. If you want to know more, check [here](https://docs.platformio.org/en/latest/).

* platformio.ini


```ini
[env:LinearActuatorController]
platform = espressif32
board = m5stack-atom
framework = arduino
monitor_speed = 115200
lib_deps =
	m5stack/M5Atom@^0.1.0
	fastled/FastLED@^3.5.0
  https://github.com/MasterAkari/LinearActuatorController.git
```

* main.cpp

```c++
#include <linear_actuator_controller.hpp>

LinearActuatorController lac;

void setup()
{
    lac.begin();

}

void loop()
{
  // move position : 0[%]
  lac.move(0);
  delay(1000);
  // move position : 100[%]
  lac.move(100);
  delay(1000);
}
```

## Usage

* SPIFFSに情報を格納する予定です。情報をフォーマットすることがあります。

## Requirement

This system uses the following libraries.

* [m5stack/M5Atom](https://github.com/m5stack/M5Atom?utm_source=platformio&utm_medium=piohome)
* [fastled/FastLED](https://github.com/Makuna/NeoPixelBus?utm_source=platformio&utm_medium=piohome)

## Changelog

It is listed [here](./Changelog).

## Notes

* This system is not guaranteed to work.

## Support

* Not Supported.

## Roadmap

* ペンプロッタ拡張キット用のクラスの追加 : (next ver.0.2.0)
* WiFiで操作するためのUIの追加 : (next ver.0.3.0)
* WiFiからペンプロッタのサーボの初期位置設定 : (next ver.0.4.0)
* WiFiからGコードなどの描画するデータを渡す機能 : (next ver.0.5.0)

<!--
* Update irregularly.

<div style="display: flex">
    <div style="width:1em; background-color: red;margin-right:1em;"></div>
    <span style="">Next Update&nbsp;:&nbsp; </span>
    <span style="">No plans.</span>
</div>

-->

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.
Please make sure to update tests as appropriate.

## Authors and acknowledgment

Show your appreciation to those who have contributed to the project.

## License

[MIT License](./LICENSE)

---

<style>
  body {
    counter-reset: chapter;
}

h2 {
    counter-reset: sub-chapter;
}

h3 {
    counter-reset: section;
}

h4 {
    counter-reset: indexlist;
}

h1::before {
    counter-reset: chapter;
}

h2::before {
    counter-increment: chapter;
    content: counter(chapter) ". ";
}

h3::before {
    counter-increment: sub-chapter;
    content: counter(chapter) "-" counter(sub-chapter) ". ";
}

h4::before {
    counter-increment: section;
    content: counter(chapter) "-" counter(sub-chapter) "-" counter(section) ". ";
}

h5::before {
    counter-increment: indexlist;
    content: "(" counter(indexlist) ") ";
}

#sidebar-toc-btn {
    bottom: unset;
    top: 8px;
}

.markdown-preview.markdown-preview {
    h2 {
        border-bottom: 4px solid #eaecef;
    }

    h3 {
        border-bottom: 1px solid #eaecef;
    }
}

.md-sidebar-toc.md-sidebar-toc {
    padding-top: 40px;
}

#sidebar-toc-btn {
    bottom: unset;
    top: 8px;
}
</style>
