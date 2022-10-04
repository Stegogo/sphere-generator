# sphere-generator
 ```
            _                                   
  ___ _ __ | |__   ___ _ __ ___  __ _  ___ _ __  
/ __| '_ \| '_ \ / _ \ '__/ _ \/ _` |/ _ \ '_ \ 
\__ \ |_) | | | |  __/ | |  __/ (_| |  __/ | | |
|___/ .__/|_| |_|\___|_|  \___|\__, |\___|_| |_|
    |_|                        |___/            
```
Spheregen is a command line tool which generates an ASCII STL file of a sphere
with the parameters you may specify. It may be useful if you need to
generate a quick 3D model for some sort of test. Enjoy!

# Running

1. Clone the repo

2. ```cd build/bin```

3. ```./spheregen -h```

# Examples

```./spheregen -R 1.5 -r 8 -s 10```

<p>
Radius: 1.5 <br>
Horizontal segments: 8 <br>
Vertical segments: 10
</p>

![image](https://user-images.githubusercontent.com/73101718/193829218-d646a483-e7ee-4f09-9fee-4571871134ed.png)

```./spheregen -R 0.5 -r 100 -s 100```
<p>
Radius: 0.5 <br>
Horizontal segments: 100 <br>
Vertical segments: 100
</p>

![image](https://user-images.githubusercontent.com/73101718/193829328-f52766c3-f467-408e-8adb-0008641c0456.png)

