EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev "1"
Comp ""
Comment1 "Designed for AISLER 2-Layer Service"
Comment2 "Part of the Mini-Mapper project"
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L mini-mapper:TB67H450FNG U2
U 1 1 5EF667BD
P 4000 3350
F 0 "U2" H 4200 3825 50  0000 C CNN
F 1 "TB67H450FNG" H 4000 3500 50  0000 C CNN
F 2 "mini-mapper:SOIC127P600X175-9N" H 4100 3300 50  0001 C CNN
F 3 "https://www.mouser.com/pdfDocs/TB67H450FNG_datasheet_en_20190401.pdf" H 4100 3300 50  0001 C CNN
	1    4000 3350
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:Motor_DC M1
U 1 1 5EF670DE
P 5250 3325
F 0 "M1" H 5397 3321 50  0000 L CNN
F 1 "Motor_DC" H 5397 3230 50  0000 L CNN
F 2 "" H 5250 3235 50  0001 C CNN
F 3 "~" H 5250 3235 50  0001 C CNN
	1    5250 3325
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 3350 4750 3350
Wire Wire Line
	4750 3350 4750 3075
Wire Wire Line
	4750 3075 5250 3075
Wire Wire Line
	5250 3075 5250 3125
Wire Wire Line
	4400 3450 4750 3450
Wire Wire Line
	4750 3450 4750 3675
Wire Wire Line
	4750 3675 5250 3675
Wire Wire Line
	5250 3675 5250 3625
$Comp
L power:GND #PWR010
U 1 1 5EF680BB
P 4000 4400
F 0 "#PWR010" H 4000 4150 50  0001 C CNN
F 1 "GND" H 4005 4227 50  0000 C CNN
F 2 "" H 4000 4400 50  0001 C CNN
F 3 "" H 4000 4400 50  0001 C CNN
	1    4000 4400
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:R R3
U 1 1 5EF6879B
P 4200 4125
F 0 "R3" H 4270 4171 50  0000 L CNN
F 1 "400m" H 4270 4080 50  0000 L CNN
F 2 "mini-mapper:R_0805" V 4130 4125 50  0001 C CNN
F 3 "~" H 4200 4125 50  0001 C CNN
	1    4200 4125
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 3975 4200 3950
Wire Wire Line
	4200 4275 4200 4350
Wire Wire Line
	4200 4350 4000 4350
Connection ~ 4000 4350
Wire Wire Line
	4000 4350 4000 4400
Text Notes 3625 4800 0    50   Italic 10
Current sense resistor
$Comp
L mini-mapper:VM #PWR09
U 1 1 5EF69E1B
P 4000 2625
F 0 "#PWR09" H 3800 2475 50  0001 C CNN
F 1 "VM" H 4015 2798 50  0000 C CNN
F 2 "" H 4000 2625 50  0001 C CNN
F 3 "" H 4000 2625 50  0001 C CNN
	1    4000 2625
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 2625 4000 2675
$Comp
L mini-mapper:Conn_01x02 J1
U 1 1 5EF7C04A
P 1800 3450
F 0 "J1" H 1718 3125 50  0000 C CNN
F 1 "Conn_01x02" H 1718 3216 50  0000 C CNN
F 2 "" H 1800 3450 50  0001 C CNN
F 3 "~" H 1800 3450 50  0001 C CNN
	1    1800 3450
	-1   0    0    1   
$EndComp
Wire Wire Line
	2000 3350 3600 3350
Wire Wire Line
	3600 3450 2000 3450
$Comp
L mini-mapper:C C3
U 1 1 5EF81780
P 3375 2875
F 0 "C3" H 3490 2921 50  0000 L CNN
F 1 "0.1U" H 3490 2830 50  0000 L CNN
F 2 "" H 3413 2725 50  0001 C CNN
F 3 "~" H 3375 2875 50  0001 C CNN
	1    3375 2875
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:C C2
U 1 1 5EF81A93
P 3150 4475
F 0 "C2" H 3265 4521 50  0000 L CNN
F 1 "0.1U" H 3265 4430 50  0000 L CNN
F 2 "" H 3188 4325 50  0001 C CNN
F 3 "~" H 3150 4475 50  0001 C CNN
	1    3150 4475
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:R R1
U 1 1 5EF81CFE
P 2750 3975
F 0 "R1" H 2820 4021 50  0000 L CNN
F 1 "7K" H 2820 3930 50  0000 L CNN
F 2 "mini-mapper:R_0805" V 2680 3975 50  0001 C CNN
F 3 "~" H 2750 3975 50  0001 C CNN
	1    2750 3975
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:R R2
U 1 1 5EF821E0
P 2750 4375
F 0 "R2" H 2820 4421 50  0000 L CNN
F 1 "2K" H 2820 4330 50  0000 L CNN
F 2 "mini-mapper:R_0805" V 2680 4375 50  0001 C CNN
F 3 "~" H 2750 4375 50  0001 C CNN
	1    2750 4375
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:CP C1
U 1 1 5EF82B68
P 2875 2875
F 0 "C1" H 2993 2921 50  0000 L CNN
F 1 "100U" H 2993 2830 50  0000 L CNN
F 2 "" H 2913 2725 50  0001 C CNN
F 3 "~" H 2875 2875 50  0001 C CNN
	1    2875 2875
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR06
U 1 1 5EF832CB
P 3375 3075
F 0 "#PWR06" H 3375 2825 50  0001 C CNN
F 1 "GND" H 3380 2902 50  0000 C CNN
F 2 "" H 3375 3075 50  0001 C CNN
F 3 "" H 3375 3075 50  0001 C CNN
	1    3375 3075
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR04
U 1 1 5EF83951
P 2875 3075
F 0 "#PWR04" H 2875 2825 50  0001 C CNN
F 1 "GND" H 2880 2902 50  0000 C CNN
F 2 "" H 2875 3075 50  0001 C CNN
F 3 "" H 2875 3075 50  0001 C CNN
	1    2875 3075
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR05
U 1 1 5EF83D45
P 3150 4700
F 0 "#PWR05" H 3150 4450 50  0001 C CNN
F 1 "GND" H 3155 4527 50  0000 C CNN
F 2 "" H 3150 4700 50  0001 C CNN
F 3 "" H 3150 4700 50  0001 C CNN
	1    3150 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3150 4700 3150 4650
Wire Wire Line
	3150 4325 3150 4175
Wire Wire Line
	3375 3075 3375 3025
Wire Wire Line
	2875 3075 2875 3025
Wire Wire Line
	2875 2725 2875 2675
Wire Wire Line
	3375 2725 3375 2675
Connection ~ 4000 2675
Wire Wire Line
	4000 2675 4000 2850
$Comp
L mini-mapper:VM #PWR03
U 1 1 5EF87B52
P 2750 3775
F 0 "#PWR03" H 2550 3625 50  0001 C CNN
F 1 "VM" H 2765 3948 50  0000 C CNN
F 2 "" H 2750 3775 50  0001 C CNN
F 3 "" H 2750 3775 50  0001 C CNN
	1    2750 3775
	1    0    0    -1  
$EndComp
Wire Wire Line
	2750 3775 2750 3825
Wire Wire Line
	2750 4125 2750 4175
Connection ~ 2750 4175
Wire Wire Line
	2750 4175 2750 4225
Wire Wire Line
	2750 4175 3150 4175
Wire Wire Line
	3150 3700 3150 4175
Connection ~ 3150 4175
Wire Wire Line
	3150 3700 3600 3700
Wire Wire Line
	2750 4650 3150 4650
Wire Wire Line
	2750 4525 2750 4650
Connection ~ 3150 4650
Wire Wire Line
	3150 4650 3150 4625
Text Notes 1125 3850 0    50   Italic 10
Current limiting reference voltage
Wire Wire Line
	2875 2675 3375 2675
Connection ~ 3375 2675
$Comp
L power:PWR_FLAG #FLG01
U 1 1 5EF90A9E
P 2375 6150
F 0 "#FLG01" H 2375 6225 50  0001 C CNN
F 1 "PWR_FLAG" V 2375 6278 50  0000 L CNN
F 2 "" H 2375 6150 50  0001 C CNN
F 3 "~" H 2375 6150 50  0001 C CNN
	1    2375 6150
	0    1    1    0   
$EndComp
$Comp
L power:PWR_FLAG #FLG02
U 1 1 5EF90FCA
P 2375 6350
F 0 "#FLG02" H 2375 6425 50  0001 C CNN
F 1 "PWR_FLAG" V 2375 6478 50  0000 L CNN
F 2 "" H 2375 6350 50  0001 C CNN
F 3 "~" H 2375 6350 50  0001 C CNN
	1    2375 6350
	0    1    1    0   
$EndComp
Text Notes 4950 2975 0    50   ~ 10
MOTOR OUTPUT
Text Notes 6000 4925 0    50   ~ 0
Op-amp gain of 30 ⇒\n   ADC input 0 - 3 V\n\nGain = 1 + Rf / Rg ⇒\n  Rf / Rg = 29\n\nRf = 330 kΩ\nRg = 12 kΩ // 220 kΩ
$Comp
L mini-mapper:Conn_01x02 J2
U 1 1 5EF91CF7
P 1800 6300
F 0 "J2" H 1718 5975 50  0000 C CNN
F 1 "Conn_01x02" H 1718 6066 50  0000 C CNN
F 2 "" H 1800 6300 50  0001 C CNN
F 3 "~" H 1800 6300 50  0001 C CNN
	1    1800 6300
	-1   0    0    1   
$EndComp
$Comp
L mini-mapper:VM #PWR01
U 1 1 5EF924B9
P 2375 6050
F 0 "#PWR01" H 2175 5900 50  0001 C CNN
F 1 "VM" H 2390 6223 50  0000 C CNN
F 2 "" H 2375 6050 50  0001 C CNN
F 3 "" H 2375 6050 50  0001 C CNN
	1    2375 6050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 5EF92821
P 2375 6400
F 0 "#PWR02" H 2375 6150 50  0001 C CNN
F 1 "GND" H 2380 6227 50  0000 C CNN
F 2 "" H 2375 6400 50  0001 C CNN
F 3 "" H 2375 6400 50  0001 C CNN
	1    2375 6400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2375 6400 2375 6350
Wire Wire Line
	2375 6300 2000 6300
Wire Wire Line
	2000 6200 2375 6200
Wire Wire Line
	2375 6200 2375 6150
Connection ~ 2375 6150
Wire Wire Line
	2375 6150 2375 6050
Wire Wire Line
	3375 2675 4000 2675
Connection ~ 2375 6350
Wire Wire Line
	2375 6350 2375 6300
Wire Wire Line
	4000 3900 4000 4350
Text Notes 1575 5850 0    50   ~ 10
POWER INPUT
Text Notes 1575 3000 0    50   ~ 10
CONTROL INPUT
Text Label 3200 3700 0    50   ~ 0
Vref
$Comp
L mini-mapper:TLV9051 U1
U 1 1 5EF98393
P 5025 4050
F 0 "U1" H 5025 4275 50  0000 C CNN
F 1 "TLV9051" H 5025 3800 50  0000 C CNN
F 2 "" H 5025 4050 50  0001 C CNN
F 3 "www.st.com/resource/en/datasheet/tsv911.pdf" H 5025 4050 50  0001 C CNN
	1    5025 4050
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:R R6
U 1 1 5EF99706
P 5500 4775
F 0 "R6" H 5430 4729 50  0000 R CNN
F 1 "220K" H 5430 4820 50  0000 R CNN
F 2 "mini-mapper:R_0805" V 5430 4775 50  0001 C CNN
F 3 "~" H 5500 4775 50  0001 C CNN
	1    5500 4775
	-1   0    0    1   
$EndComp
$Comp
L mini-mapper:R R5
U 1 1 5EF99DCA
P 5400 4300
F 0 "R5" H 5330 4254 50  0000 R CNN
F 1 "330K" H 5330 4345 50  0000 R CNN
F 2 "mini-mapper:R_0805" V 5330 4300 50  0001 C CNN
F 3 "~" H 5400 4300 50  0001 C CNN
	1    5400 4300
	-1   0    0    1   
$EndComp
Wire Wire Line
	5325 4050 5400 4050
Wire Wire Line
	5400 4050 5400 4150
Wire Wire Line
	5400 4450 5400 4475
$Comp
L power:GND #PWR011
U 1 1 5EF9C41A
P 5400 5075
F 0 "#PWR011" H 5400 4825 50  0001 C CNN
F 1 "GND" H 5405 4902 50  0000 C CNN
F 2 "" H 5400 5075 50  0001 C CNN
F 3 "" H 5400 5075 50  0001 C CNN
	1    5400 5075
	1    0    0    -1  
$EndComp
Wire Wire Line
	4725 4150 4650 4150
Wire Wire Line
	4650 4150 4650 4475
Wire Wire Line
	4650 4475 5400 4475
Connection ~ 5400 4475
Wire Wire Line
	4725 3950 4200 3950
Connection ~ 4200 3950
Wire Wire Line
	4200 3950 4200 3900
Wire Wire Line
	5400 4050 6150 4050
Connection ~ 5400 4050
$Comp
L mini-mapper:TLV9051 U1
U 2 1 5EFA26E1
P 3775 6200
F 0 "U1" H 3803 6246 50  0000 L CNN
F 1 "TLV9051" H 3803 6155 50  0000 L CNN
F 2 "" H 3775 6200 50  0001 C CNN
F 3 "www.st.com/resource/en/datasheet/tsv911.pdf" H 3775 6200 50  0001 C CNN
	2    3775 6200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR08
U 1 1 5EFA3D5E
P 3675 6575
F 0 "#PWR08" H 3675 6325 50  0001 C CNN
F 1 "GND" H 3680 6402 50  0000 C CNN
F 2 "" H 3675 6575 50  0001 C CNN
F 3 "" H 3675 6575 50  0001 C CNN
	1    3675 6575
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:VM #PWR07
U 1 1 5EFA41B3
P 3675 5825
F 0 "#PWR07" H 3475 5675 50  0001 C CNN
F 1 "VM" H 3690 5998 50  0000 C CNN
F 2 "" H 3675 5825 50  0001 C CNN
F 3 "" H 3675 5825 50  0001 C CNN
	1    3675 5825
	1    0    0    -1  
$EndComp
Wire Wire Line
	3675 5825 3675 5900
Wire Wire Line
	3675 6500 3675 6575
Text Notes 5775 4325 0    50   ~ 0
Rf
Text Notes 5775 4775 0    50   ~ 0
Rg
$Comp
L mini-mapper:R R4
U 1 1 5EFAA4AA
P 5300 4775
F 0 "R4" H 5475 4725 50  0000 R CNN
F 1 "12K" H 5525 4825 50  0000 R CNN
F 2 "mini-mapper:R_0805" V 5230 4775 50  0001 C CNN
F 3 "~" H 5300 4775 50  0001 C CNN
	1    5300 4775
	-1   0    0    1   
$EndComp
Wire Wire Line
	5400 4575 5500 4575
Wire Wire Line
	5500 4575 5500 4625
Wire Wire Line
	5400 4475 5400 4575
Wire Wire Line
	5300 4625 5300 4575
Wire Wire Line
	5300 4575 5400 4575
Connection ~ 5400 4575
Wire Wire Line
	5300 4925 5300 5000
Wire Wire Line
	5300 5000 5400 5000
Wire Wire Line
	5500 5000 5500 4925
Wire Wire Line
	5400 5075 5400 5000
Connection ~ 5400 5000
Wire Wire Line
	5400 5000 5500 5000
$Comp
L mini-mapper:Conn_01x01 J3
U 1 1 5EFC82DA
P 6350 4050
F 0 "J3" H 6430 4092 50  0000 L CNN
F 1 "Conn_01x01" H 6430 4001 50  0000 L CNN
F 2 "" H 6350 4050 50  0001 C CNN
F 3 "~" H 6350 4050 50  0001 C CNN
	1    6350 4050
	1    0    0    -1  
$EndComp
Text Notes 6075 3875 0    50   ~ 10
CURRENT SENSE OUTPUT
Text Label 2400 3350 0    50   ~ 0
IN1
Text Label 2400 3450 0    50   ~ 0
IN2
Text Label 5850 4050 0    50   ~ 0
Vsense
Text Notes 3625 5400 0    50   ~ 0
Current: 0 ≤ I ≤ 250 mA\nKeep Vsense ≤ 100 mV\n\nRsense = 0.4 Ω ⇒\n   0 ≤ Vsense ≤ 0.1 V\n\n(Pmax = 25 mW)\n
Text Notes 1125 4575 0    50   ~ 0
Iout(max) = 0.1 Vref / Rsense\n\nWant Iout(max) = 250 mA\nwith VM = 4.5 V\n\nSetting Rsense = 0.4 Ω\n\nVref = 10 x 0.25 x 0.4 = 1 V\n
$EndSCHEMATC
