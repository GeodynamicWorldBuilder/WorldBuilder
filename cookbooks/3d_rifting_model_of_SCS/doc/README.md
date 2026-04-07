(part:user_manual:chap:cookbooks:sec:3d_rifting_model_of_SCS)=
3D Rifting Model of the South China Sea
=========================

This model is designed based on the northwestern continental margin of the South China Sea. It incorporates key tectonic units including the continental margin, islands, oceanic crust, and a mantle plume. The geometries of different plates in the model were digitized using ArcMap. 

To facilitate numerical simulations and rheological parameter assignment, each structural unit is defined with distinct compositional fields.
The continental margin represents typical passive margin basins, mainly corresponding to the Qiongdongnan Basin and the Pearl River Mouth Basin. 
Three island blocks are included in the model, representing Hainan Island, Xisha Islands, and Zhongsha Islands. These regions are characterized by significantly thickened lithosphere compared to the surrounding margin.
The oceanic domain represents the Northwestern Sub-basin and Eastern Sub-basin of the South China Sea. It is modeled as relatively thin oceanic lithosphere and includes two orthogonal spreading ridge systems to approximate seafloor spreading fabric.
A mantle plume is introduced as Hainan plume. The plume is defined by a moderate positive thermal anomaly relative to the ambient mantle.
In addition, a transitional boundary with gradual variations in both thickness and temperature is set up using `max depth`.
All model parameters can be adjusted depending on specific research objectives.

::::{tab-set}
:::{tab-item} Root model information
```{literalinclude} ../3d_rifting_model_of_SCS.wb
:language: json
:lineno-start: 29
:lines: 29-144
```
:::
:::{tab-item} Full File
```{literalinclude} ../3d_rifting_model_of_SCS.wb
:language: json
:lineno-start: 1
```
:::
::::


The figure below show the result of the model. 

```{figure} ./GWB_SCS.jpg
:name: GWB_SCS
:alt: 3d rifting model of SCS. 
:align: center

Green regions denote the island blocks, characterized by relatively thick lithosphere. 
Yellow regions represent the continental margin domain, while blue regions correspond to the oceanic domain with ridge systems developed in two directions. 
The red region indicates the location of the Hainan mantle plume.
Continental Margin：upper crust 15 km, lower crust 17 km, LAB 70 km;
Island Blocks: upper crust 20 km, lower crust 20 km, LAB 120 km;
Oceanic Domain:	crust 15 km, LAB 35 km.
```

It should be emphasized that these represent a present-day configuration and are not the initial model geometry, but are used for model setup testing.
