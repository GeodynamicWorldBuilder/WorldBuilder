(part:user_manual:chap:cookbooks:sec:3d_rifting_model_of_SCS)=
3D Rifting Model of the South China Sea
=========================

This model is designed based on the northwestern continental margin of the South China Sea. It incorporates key tectonic units including the continental margin, islands, oceanic crust, and a mantle plume. The geometries of different plates in the model were digitized using ArcMap. 

To facilitate numerical simulations and rheological parameter assignment, each structural unit is defined with distinct compositional fields.
The continental margin represents typical passive margin basins, mainly corresponding to the Qiongdongnan Basin and the Pearl River Mouth Basin. 
Three island blocks are included in the model, representing Hainan Island, Xisha Islands, and Zhongsha Islands. These regions are characterized by significantly thickened lithosphere compared to the surrounding margin.
The oceanic domain represents the Northwestern sub-basin and Eastern sub-basin of the South China Sea. It is modeled as relatively thin oceanic lithosphere and includes two orthogonal spreading ridge systems to approximate seafloor spreading fabric.
A mantle plume is introduced as Hainan plume. The plume is defined by a moderate positive thermal anomaly relative to the ambient mantle.
In addition, a transitional boundary with gradual variations in both thickness and temperature is set up using `max depth`.
All model parameters can be adjusted depending on specific research objectives.

::::{tab-set}
:::{tab-item} Root model information
```{literalinclude} ../3d_rifting_model_of_SCS.wb
:language: json
:lineno-start: 15
:lines: 15-60
```
:::
:::{tab-item} Full File
```{literalinclude} ../3d_rifting_model_of_SCS.wb
:language: json
:lineno-start: 1
```
:::
::::


The figure below shows a geological map for model reference. 

```{figure} ./GWB_SCS_R.jpg
:name: GWB_SCS_R
:alt: 3d rifting model of SCS. 
:align: center

The simplified tectonic map of the northern margin of the South China Sea, modified from Zhang et al. (2019). The location of the Hainan mantle plume in the model is taken from Xia et al. (2025).
Zhang, Y., Qi, J., and Wu, J. Cenozoic Faults Systems and Its Geodynamics of the Continental Margin Basins in the Northern of South China Sea. Earth Science. 2019, 44(2): 603-625 (in Chinese with English abstract).
Xia, S., Guo, T., Zhao, F., et al. Late Cenozoic magmatism of the South China Sea driven by the Hainan mantle plume. Science Bulletin, 2025, 70: 604-608.
```

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
