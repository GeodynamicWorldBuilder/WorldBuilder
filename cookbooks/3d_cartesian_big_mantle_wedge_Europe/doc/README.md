(part:user_manual:chap:cookbooks:sec:3d_cartesian_big_mantle_wedge_Europe)=
Big Mantle Wedge in Europe
======================

Intra-continental volcanoes differ significantly from their oceanic counterparts (e.g., Hawaii and Iceland) because: (1) their eruptions are sporadic (every few thousand years), (2) they do not display the classic age progression associated with a hot spot, and (3) their eruption products are more alkaline and silica-undersaturated compared to oceanic island basalts. Moreover, seismic tomography beneath these regions reveals that the low-velocity anomalies commonly associated with warm mantle plumes are interrupted in the mantle transition zone (MTZ) by high-velocity anomalies {cite:p}`Zhao_etal_2004`. These anomalies have been interpreted as a cold, stagnant slab in the MTZ.

In the Big Mantle Wedge (BMW) hypothesis {cite:p}`Zhao_etal_2009`, intra-continental volcanism is either induced by the bulldozing action of a subducting slab pushing hydrous material out of the MTZ {cite:p}`Yang_&_Faccenda_2020`, or by the dehydration of the slab itself once it stagnates in the MTZ and warms up {cite:p}`Xing_etal_2024`. 

In the Mediterranean region there are several volcanic provinces which are not related to either converging or diverging margin, and are collectively referred to as the Circum-Mediterranean Anorogenic Cenozoic Igneous Province (CiMACI) {cite:p}`Lustrino_&_Wilson_2007`. Within the CiMACI, there are four volcanic provinces located north of the Alps: (1) Massif Central (France), (2) Eifel (Germany), (3) Eger Rift (Czech Republic), and (4) the Pannonian Basin (Hungary). These four regions together constitute the European Cenozoic Rift system (ECRiS) {cite:p}`Ziegler_1992`. 

Seismic tomography of Europe reveals slow-velocity anomalies in the upper mantle beneath the volcanic provinces of the ECRiS and fast-velocity anomalies in the underlying MTZ {cite:p}`Piromallo_&_Faccenna_2004`, which have been interpreted as different generations of Mediterranean slabs now stagnating on top of the 660 km discontinuity {cite:p}`Piromallo_&_Faccenna_2004`. Potentially, the four volcanic regions of the ECRiS are fed by a common mantle reservoir {cite:p}`Granet_etal_1995`, represented by the recycling (i.e., flux melting) of subducted oceanic crust (meta-sediments and meta-basalts) {cite:p}`Yaxley_etal_2022`, induced by the dehydration of the inner portions of slab {cite:p}`Xing_etal_2024`. This process may lead to the formation of secondary mantle plumes driven by the chemical buoyancy of low-density, water-rich melt {cite:p}`Sakamaki_&_Ohtani_2022` that ascends toward the surface and may be responsible for the slow-velocity anomaly observed in the upper mantle beneath ECRiS.

The Big Mantle Wedge hypothesis was originally proposed to explain the intra-continental volcanism in the Changbaishan volcanic province (NE China), located more than 1000 km away from the Japan Trench {cite:p}`Zhao_etal_2004`, and may also explain the intra-continental volcanism of the ECRiS.

This cookbook can be used to set up the initial conditions of a regional-scale mantle convection model to investigate the Big Mantle Wedge hypothesis or to visualize its geometry in 3D.

The region is 4000 x 3000 x 660 km Cartesian box (x,y,z) representing continental Europe and the underlying upper mantle and transition zone (Figure 1-3). In total there are 15 features, divided as:

1. Mantle Layer: uniform composition [0]; no distinction between upper mantle and MTZ

2. Oceanic Plate: relatively thin lithosphere (100 km), uniform composition [1]; it represents the ocean and seas surrounding Europe

3. Continental Plates 

    3a. Continental Europe: relatively thick lithosphere (200 km), uniform composition [2]

    3b. Alps: mountain chain, thick lithosphere (250 km), uniform composition [3]

    3c. Appennines: mountain chain, thick lithosphere (250 km), uniform composition [3]

    3d. Pyrenees: mountain chain, thick lithosphere (250 km), uniform composition [3]

    3e. Massif Central: ECRiS volcanic province; thinned continental lithosphere (100 km), uniform composition [4]

    3f. Eifel: ECRiS volcanic province; thinned continental lithosphere (100 km), uniform composition [4]

    3g. Bohemian Massif: ECRiS volcanic province; thinned continental lithosphere (100 km), uniform composition [4]

    3h. Pannonian Basin: ECRiS volcanic province; thinned continental lithosphere (100 km), uniform composition [4]
 
4. Subducting Slab (Alpine): defined by 5 segments to represent the steep descent toward the MTZ and the horizontal flattening of the slab 
                            along the 660 km discontinuity. The final segment stretches north-ward to reach the four ECRiS regions. Uniform composition [4]. The coordinates of the linear feature are taken in the middle of the Alps polygon.

5. Mantle Plumes: the [x,y] coordinates of the four plumes are defined by the centroids of the polygons created to represent the ECRiS 
                 regions. Uniform composition [5]. The maximum depth of each plume has been limited to < 600 km to represent the secondary plumes generated in the MTZ by slab dehydration.

    5a. Massif Central Plume

    5b. Eifel Plume

    5c. Boehmian Massif Plume

    5d. Pannonian Plume

:::{figure}  big_mantle_wedge_Europe_surface_view.png
:name:  big_mantle_wedge_Europe_surface_view
:alt: European continental plate including the Alps, Appennines, Pyrenees and the ECRiS regions. 
:align: center
:::

Figure (1): different colours represent different features: oceanic plate (dark blue), continental plate (dark green), mountain ranges (light green), intra-continental volcanic provinces (orange)

:::{figure}  big_mantle_wedge_Europe_mantle_view.png
:name:  big_mantle_wedge_Europe_mantle_view
:alt: Alpine slab subducting beneath Central Europe 
:align: center
:::

Figure (2): different colours represent different features: oceanic plate (dark blue), continental plate (dark green), mountain ranges (light green), and subducted slab (orange)

:::{figure}  big_mantle_wedge_Europe_plumes_view.png
:name:  big_mantle_wedge_Europe_plumes_view
:alt: Mantle plumes feeding the ECRiS intra-continental volcanic regions 
:align: center
:::

Figure (3): different colours represent different features: oceanic plate (dark blue), continental plate (dark green), mountain ranges (light green), subducted slab (orange), intra-continental volcanic provinces and ascending plumes (red)
