# Photoencoder selection

## Type

 - Two options: phototransistor or "photo IC".
 - Phototransistor is what it says => outputs current signal depending
   on degree of occlusion of optical beam.
 - "Photo IC" is usually a photodiode, an amplifier, a Schmitt trigger
   and an output transistor => does thresholding and outputs logic
   levels. [EXAMPLE FROM TT ELECTRONICS DATASHEET]
 - Phototransistor more flexible in terms of handling partial
   occlusion/non-occlusion of optical beam; with photo IC parts, you
   need to alternate between complete occlusion and complete
   non-occlusion, I think.
 - So, use a phototransistor device.


## Electrical specifications


## Physical envelope

Need to know:

 - Length of motor output shaft (ca. 6mm) [MEASURE!!!]
 - Thickness of encoder disk material (ca. 1mm)
 - Diameter of encoder disk (22.5mm)
 - Radius of circle of centres for encoder disk holes (10mm)
 - Diameter of encoder disk holes (1mm)
 - Physical dimensions of encoders: L (along optical axis) x W
   (perpendicular to optical axos) x H (from PCB to top of encoder
   vertically), slot width (Ws) and depth (Hs), optical axis depth
   from top of encoder (Ho)
