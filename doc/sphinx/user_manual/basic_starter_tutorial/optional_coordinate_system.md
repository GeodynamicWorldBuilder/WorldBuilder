Optional parameter: coordinate system
=====================================

```{todo}
Explain that the coordinate system is an optional parameter which by default is set to Cartesian.
```


This is what we had in our minimal example.
```{code-block} json
---
lineno-start: 1
---
{
    "version":"0.3",
    "features":[ ]
}
```


And now we add one line and set it to the default value. There is no difference between this one and the previous code block. 
```{code-block} json
---
lineno-start: 1
---
{
    "version":"0.3",
    "coordinate system":{"model":"cartesian"},
    "features":[ ]
}
```