# Isophine

A blazing fast PDF imposition GUI tool written in C++ with Qt and QPDF.

If you're interested in this project, please read the
[contribution](#contribution) section of this README.

## Preview

## Compilation

This project requires Qt6 and QPDF as dependencies for compilation.
Please download them before compiling. Follow the commands below to compile
the project. The executable should be present at `./build/src/isophine`.

```sh
git clone --recursive git@github.com:Bubu-Droid/isophine.git
cd isophine
mkdir build
cmake -B build
cmake --build build
```

## Configuration

You can edit the app's configuration from within the app.
It can also be edited by editing the `.ini` configuration file.

Linux path: `~/.config/isophine/isophine.ini`.

### Default Keybindings and Shortcuts

All of these keybindings can changed be from the app settings.
The default keybindings are inspired by vim motion keybindings.

| Keys     | Description                                    |
| -------- | ---------------------------------------------- |
| `n`      | Move to the next page                          |
| `p`      | Move to the previous page                      |
| `h`      | Move the page left by small amount             |
| `H`      | Move the page left by large amount             |
| `j`      | Move the page down by small amount             |
| `J`      | Move the page down by large amount             |
| `k`      | Move the page up by small amount               |
| `K`      | Move the page up by large amount               |
| `l`      | Move the page right by small amount            |
| `L`      | Move the page right by large amount            |
| `r`      | Rotate the page clockwise by small amount      |
| `R`      | Rotate the page clockwise by large amount      |
| `e`      | Rotate the page anti-clockwise by small amount |
| `E`      | Rotate the page anti-clockwise by large amount |
| `=`      | Scale the page up by small amount              |
| `+`      | Scale the page up by large amount              |
| `-`      | Scale the page down by small amount            |
| `_`      | Scale the page down by large amount            |
| `Ctrl++` | Zoom the canvas in                             |
| `Ctrl+-` | Zoom the canvas out                            |
| `Ctrl+V` | Toggle thumbnail view panel                    |
| `Ctrl+I` | Toggle inspector panel                         |
| `Ctrl+Q` | Quit the application                           |
| `Ctrl+H` | Go to dashboard from editor                    |
| `Ctrl+N` | Create new project in dashboard                |
| `Ctrl+O` | Open existing project from dashboard           |
| `Ctrl+,` | Open app settings                              |

### Default Values for Other Settings Variables

| Key                                    | Value     |
| -------------------------------------- | --------- |
| Translate small amount                 | 1         |
| Translate large amount                 | 10        |
| Scale small amount                     | 0.01      |
| Scale large amount                     | 0.1       |
| Rotate small amount                    | 0.1       |
| Rotate large amount                    | 1         |
| Horizontal lines count of bounding box | 5         |
| Vertical lines count of bounding box   | 3         |
| Initial scale of bouding box           | 1         |
| Default gridline color of bounding box | Dark blue |

> [!TIP]
> If your configuration file is malformed/corrupted, you may copy
> the default configuration file given below and overwrite the content
> of the current configuration file.

<details><summary>Default Configuration for `.ini` file</summary>

```ini
    [BoundingBox]
    HorizontalLinesCount=5
    LineColor=@Variant(\0\0\0\x43\x1\xff\xff\0\0\0\0\x80\x80\0\0)
    Scale=1
    VerticalLinesCount=3

    [Increments]
    Rotate\LargeAmount=1
    Rotate\SmallAmount=0.1
    Scale\LargeAmount=0.1
    Scale\SmallAmount=0.01
    Translate\LargeAmount=10
    Translate\SmallAmount=1

    [Keybindings]
    Movement\Down\LargeKey=Shift+J
    Movement\Down\SmallKey=J
    Movement\Left\LargeKey=Shift+H
    Movement\Left\SmallKey=H
    Movement\Right\LargeKey=Shift+L
    Movement\Right\SmallKey=L
    Movement\Up\LargeKey=Shift+K
    Movement\Up\SmallKey=K
    Navigation\NextKey=N
    Navigation\PrevKey=P
    Rotate\AntiClockwise\LargeKey=Shift+E
    Rotate\AntiClockwise\SmallKey=E
    Rotate\Clockwise\LargeKey=Shift+R
    Rotate\Clockwise\SmallKey=R
    ScalePage\Down\LargeKey=Shift+_
    ScalePage\Down\SmallKey=-
    ScalePage\Up\LargeKey=Shift++
    ScalePage\Up\SmallKey="="
```

</details>

## License

- The main code is licensed under [GPLv3](LICENSE).
- The GUI, which is built using Qt6, is licensed under GPLv3.
- The renderer, which uses QPDF, is licensed under Apache License 2.0.

## Contribution

Thanks a lot of showing interest in this project of mine.
Please read what I've written below:

This is a very early release of the project. It's my first C++ project
and also my first open source project. I barely knew about the application
of OOP concepts (like setters and getters), and by the time I got to know
where to apply these (from Qt signals and slots), it was too late.

I plan to rewrite a huge chunk of this project by the end of this year.
This is not a stable release as of now. If you're interested in contributing,
I humbly request you to wait for a couple more months for me
to release the first stable release of this project.
I'm sure that this project has loads of room
to grow, and I don't want the current hacky state of the code
to hinder its potential. So, please star this repository and open an issue
if you want to request a new feature / contribute a new feature.
I plan to work on this project again during my winter break and
hopefully I'll be able to implement what I know, properly, by then.

I plan to make an API for the renderer and also introduce a
command line tool (like CPDF) which allows to transform and clip particular
pages of the input PDF. This way, Isophine would also be ready for
implementation in scripts.
