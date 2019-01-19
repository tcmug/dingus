# Dingus

React like UI proof of concept "library" in C. Started as a shitcoding project, but it sort of ended up working _too well_ albeit in a perverse way.

[Flatpak](https://flatpak.org/) and **make install** builds are supported.

This documentation is **very much incomplete and probably constantly changing.** Let's see how deep the rabbit hole goes...

## Where to start?

**Linux tested only**. You'll need **SDL2** and **SDL2 ttf** libraries and headers installed. Flatpak build requires the **org.freedesktop.Platform** runtime.

### Makefile

The [Makefile](./Makefile) contains shorthands for creating development, production and flatpak builds. The C compilation is handled with cmake in [CMakeLists.txt](CMakeLists.txt).

#### Development

The development builds are placed under **./dev**. To create a development build, run:

```
$ make development
```

To run it:

```
$ make run
```

### Flatpak

The flatpak builds are placed under **./flatpak**. To create such a build, run:

```
$ make flatpak
```

To run it:

```
$ make flatpak-run
```

To create a distributable (placed in **./flatpak-dist**):

```
$ make flatpak-dist
```

#### Production

If you want to install the application without flatpak, you need to build a production build and install it. The production builds are placed under **./dist**. To create such a build, run:

```
$ make production
```

Note: this build cannot be run without installing it:

```
$ make install
```

Read more about the [source code](./docs/code.md).
