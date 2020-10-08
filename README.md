# CG-Project

Project for Computer Graphics subject

## Index
* [Dependencies](#Dependencies)
* [Compiling and Executing](#Compiling-and-Executing)
    * [Dependencies](#Dependencies)
    * [Compiling](#Compiling)
    * [Executing](#Executing)
* [Project Structure Orientation](#Project-Structure-Orientation)
* [Implementation Plans](#Implementation-Plans)

## Compiling and Executing

### Dependencies

* GLUT

* GLEW

* Devil

### Compiling
To compile the primitive generator:
```
make generator
```

To compile the 3D Rendering Engine:
```
make renderer
```

### Executing

To execute the primitive generator:
```
./bin/generator cylinder [radius] [height] [slices] [file_name]

./bin/generator sphere [radius] [slices] [stacks] [file_name]

./bin/generator plane [width] [file_name]

./bin/generator box [width_x] [width_y] [width_z] [divisions] [file_name]

./bin/generator cone [radius] [height] [slices] [stacks] [file_name]

./bin/generator ring [inner_radius] [outter_radius] [slices] [file_name]

./bin/generator torus [radius] [cyl_radius] [slices] [cyl_slices] [file_name]

./bin/generator patch [patch_file] [detail] [file_name]
```

To execute the 3D Rendering Engine:
```
./bin/renderer [xml_file]
```


## Controls

For camera spherical movement you can use the `Arrow` keys

For camera positional movement of the focus point use the `WASD` keys

For zooming in and out use `E` and `Q` respectively

Press `space` key for debug mode (shows trajectories and model mesh)

For timetravel use `-` and `+` to decrease and increase respectively

## Project Structure Orientation

`include/` is for headers (.hpp) \
`src/` is for source (.cpp)

This project is divided in 2 programs:
- The primitive generator
- The 3D Rendering "Engine"

Inside `src/` , for each of these programs we have a corresponding folder respectively:
- `generator/`
- `renderer/`

For every source that both programs use in common there will be a folder `common/`

___

The `include/lib/` folder contains external header-only libraries that were user in this project 
(Used for example in parsing XML files)

## Implementation Plans

#### Mesh [Done]
A idea of a drawable `Mesh` for painting in the screen shapes

#### Scene [Done]
`Scene` is the base collection of models and properties to be drawn
Represents a arbitrary number of 3D models and possibly 
can have other atributes like skyboxes and things like that

#### Disk [Done]
Disk will be a new primitive supported by the generator
This model will allow saturn to have rings

#### Skybox [Done]
Textured skybox

___
## Notes

- **Generator** also has a *skybox* primitive but for now we won't add it to the *shell command generator*, because it is now a necessary primitive to instanciate and just a variant of a inverted box with fixed axis width to the same value.
It can only be created directly from the generator functions