<p align="center">
    <img src="Img/logo.png">
</p>

<h1></h1>

<br>

<div style="display: inline;" align="center">
    <a href="https://github.com/rdWei/rdWei/blob/main/donate.MD">
        Support Me
    </a>
</div>

### TOC

- [Description](#description)
    - [Why](#why)
- [Showcase](#showcase)
- [Limitations](#limitations)
- [Dependencies](#dependencies)
- [Compile](#compile)
- [Usage](#usage)
- [TODO](#todo)
- [Contributing](#contributing)

## Description
C-rypt is a software written entirely in C. It enables encryption and decryption of text from an image ([Steganography](https://en.wikipedia.org/wiki/Steganography)), which can be easily shared via messaging apps like WhatsApp or Telegram. The software features a straightforward and intuitive GUI interface.

### Why
C-rypt serves as a personal project aimed at honing my skills in C programming. As I am currently learning the language, C-rypt provides a practical avenue to apply theoretical knowledge to real-world scenarios, particularly in cryptography and graphical user interface development.

## Showcase

Before            |  After
:-------------------------:|:-------------------------:
![](Img/img1.png)  |  ![](Img/img2.png)

## Limitations

- Messages support alphabetical letters and spaces.
- Supported image format: PNG.
- Maximum character count: depend on the size of the image (if x = 600 then max char = ~900)

_See [TODO](#todo)_

## Dependencies

To compile C-rypt, you will need the following libraries:

- [leif](https://github.com/cococry/leif)
- [libpng](http://www.libpng.org/pub/png/libpng.html)

### Installing Dependencies on Ubuntu

```bash
sudo apt-get update
sudo apt-get install libpng-dev
# For leif, follow the instructions in the repository
git clone https://github.com/cococry/leif.git
cd leif
make
sudo make install

