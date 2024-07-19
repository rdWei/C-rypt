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
- [Contributing](#Contributing)

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

_See [TODO](#TODO)_

## Dependencies

To compile C-rypt, you will need the following libraries:

- [leif](https://github.com/cococry/leif)
- [libpng](http://www.libpng.org/pub/png/libpng.html)


## Compile

```
mkdir build
cd build
cmake ..
make
cd ..
./out/C-rypt
```

## Usage
Here is a video that explains how to use [C-rypt](https://packaged-media.redd.it/zhfs3tceqzcd1/pb/m2-res_480p.mp4?m=DASHPlaylist.mpd&v=1&e=1721242800&s=918119cc8a91a2c42590fdf5f6eda27abea71bc7#t=0)

## Todo

- Increase maximum character limit to 1000.
- Add support for special characters such as ?, !, &, :, ., and numbers in messages.

## Contributing
If you'd like to contribute, you can submit a [pull request](https://github.com/rdWei/C-rypt/pulls) with your changes or open an [issue](https://github.com/rdWei/C-rypt/pulls) to report any problems or feature requests.

