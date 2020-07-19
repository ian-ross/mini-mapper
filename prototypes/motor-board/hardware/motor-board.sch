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
L mini-mapper:TB67H450FNG U1
U 1 1 5EF667BD
P 3650 1725
F 0 "U1" H 3850 2200 50  0000 C CNN
F 1 "TB67H450FNG" H 3650 1875 50  0000 C CNN
F 2 "mini-mapper:SOIC127P600X175-9N" H 3750 1675 50  0001 C CNN
F 3 "https://www.mouser.com/pdfDocs/TB67H450FNG_datasheet_en_20190401.pdf" H 3750 1675 50  0001 C CNN
F 4 "TB67H450FNG" H 3650 1725 50  0001 C CNN "MPN"
F 5 "https://toshiba.semicon-storage.com/us/semiconductor/product/motor-driver-ics/brushed-dc-motor-driver-ics/detail.TB67H450FNG.html" H 3650 1725 50  0001 C CNN "Link"
	1    3650 1725
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:Motor_DC #M1
U 1 1 5EF670DE
P 5200 1700
F 0 "#M1" H 5347 1696 50  0000 L CNN
F 1 "Motor_DC" H 5347 1605 50  0000 L CNN
F 2 "" H 5200 1610 50  0001 C CNN
F 3 "~" H 5200 1610 50  0001 C CNN
	1    5200 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 1450 5200 1500
Wire Wire Line
	5200 2050 5200 2000
$Comp
L power:GND #PWR08
U 1 1 5EF680BB
P 3650 2775
F 0 "#PWR08" H 3650 2525 50  0001 C CNN
F 1 "GND" H 3655 2602 50  0000 C CNN
F 2 "" H 3650 2775 50  0001 C CNN
F 3 "" H 3650 2775 50  0001 C CNN
	1    3650 2775
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:R R4
U 1 1 5EF6879B
P 3850 2500
F 0 "R4" H 3920 2546 50  0000 L CNN
F 1 "400m" H 3920 2455 50  0000 L CNN
F 2 "mini-mapper:R_0805" V 3780 2500 50  0001 C CNN
F 3 "~" H 3850 2500 50  0001 C CNN
	1    3850 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 2725 3650 2725
Connection ~ 3650 2725
Wire Wire Line
	3650 2725 3650 2775
Text Notes 3275 3175 0    50   Italic 10
Current sense resistor
$Comp
L mini-mapper:VM #PWR01
U 1 1 5EF69E1B
P 3650 1000
F 0 "#PWR01" H 3450 850 50  0001 C CNN
F 1 "VM" H 3665 1173 50  0000 C CNN
F 2 "" H 3650 1000 50  0001 C CNN
F 3 "" H 3650 1000 50  0001 C CNN
	1    3650 1000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 1000 3650 1050
$Comp
L mini-mapper:Conn_01x02 J2
U 1 1 5EF7C04A
P 1450 1825
F 0 "J2" H 1368 1500 50  0000 C CNN
F 1 "Conn_01x02" H 1368 1591 50  0000 C CNN
F 2 "" H 1450 1825 50  0001 C CNN
F 3 "~" H 1450 1825 50  0001 C CNN
	1    1450 1825
	-1   0    0    1   
$EndComp
Wire Wire Line
	1650 1725 3250 1725
Wire Wire Line
	3250 1825 1650 1825
$Comp
L mini-mapper:C C2
U 1 1 5EF81780
P 3025 1250
F 0 "C2" H 3140 1296 50  0000 L CNN
F 1 "0.1U" H 3140 1205 50  0000 L CNN
F 2 "mini-mapper:C_0805" H 3063 1100 50  0001 C CNN
F 3 "~" H 3025 1250 50  0001 C CNN
	1    3025 1250
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:C C4
U 1 1 5EF81A93
P 2800 2850
F 0 "C4" H 2915 2896 50  0000 L CNN
F 1 "0.1U" H 2915 2805 50  0000 L CNN
F 2 "mini-mapper:C_0805" H 2838 2700 50  0001 C CNN
F 3 "~" H 2800 2850 50  0001 C CNN
	1    2800 2850
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:R R3
U 1 1 5EF81CFE
P 2400 2350
F 0 "R3" H 2470 2396 50  0000 L CNN
F 1 "7K" H 2470 2305 50  0000 L CNN
F 2 "mini-mapper:R_0805" V 2330 2350 50  0001 C CNN
F 3 "~" H 2400 2350 50  0001 C CNN
	1    2400 2350
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:R R6
U 1 1 5EF821E0
P 2400 2750
F 0 "R6" H 2470 2796 50  0000 L CNN
F 1 "2K" H 2470 2705 50  0000 L CNN
F 2 "mini-mapper:R_0805" V 2330 2750 50  0001 C CNN
F 3 "~" H 2400 2750 50  0001 C CNN
	1    2400 2750
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:CP C1
U 1 1 5EF82B68
P 2525 1250
F 0 "C1" H 2643 1296 50  0000 L CNN
F 1 "100U" H 2643 1205 50  0000 L CNN
F 2 "" H 2563 1100 50  0001 C CNN
F 3 "~" H 2525 1250 50  0001 C CNN
	1    2525 1250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR04
U 1 1 5EF832CB
P 3025 1450
F 0 "#PWR04" H 3025 1200 50  0001 C CNN
F 1 "GND" H 3030 1277 50  0000 C CNN
F 2 "" H 3025 1450 50  0001 C CNN
F 3 "" H 3025 1450 50  0001 C CNN
	1    3025 1450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR03
U 1 1 5EF83951
P 2525 1450
F 0 "#PWR03" H 2525 1200 50  0001 C CNN
F 1 "GND" H 2530 1277 50  0000 C CNN
F 2 "" H 2525 1450 50  0001 C CNN
F 3 "" H 2525 1450 50  0001 C CNN
	1    2525 1450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR010
U 1 1 5EF83D45
P 2800 3075
F 0 "#PWR010" H 2800 2825 50  0001 C CNN
F 1 "GND" H 2805 2902 50  0000 C CNN
F 2 "" H 2800 3075 50  0001 C CNN
F 3 "" H 2800 3075 50  0001 C CNN
	1    2800 3075
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 3075 2800 3025
Wire Wire Line
	2800 2700 2800 2550
Wire Wire Line
	3025 1450 3025 1400
Wire Wire Line
	2525 1450 2525 1400
Wire Wire Line
	2525 1100 2525 1050
Wire Wire Line
	3025 1100 3025 1050
Connection ~ 3650 1050
Wire Wire Line
	3650 1050 3650 1225
$Comp
L mini-mapper:VM #PWR06
U 1 1 5EF87B52
P 2400 2150
F 0 "#PWR06" H 2200 2000 50  0001 C CNN
F 1 "VM" H 2415 2323 50  0000 C CNN
F 2 "" H 2400 2150 50  0001 C CNN
F 3 "" H 2400 2150 50  0001 C CNN
	1    2400 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2400 2150 2400 2200
Wire Wire Line
	2400 2500 2400 2550
Connection ~ 2400 2550
Wire Wire Line
	2400 2550 2400 2600
Wire Wire Line
	2400 2550 2800 2550
Wire Wire Line
	2800 2075 2800 2550
Connection ~ 2800 2550
Wire Wire Line
	2800 2075 3250 2075
Wire Wire Line
	2400 3025 2800 3025
Wire Wire Line
	2400 2900 2400 3025
Connection ~ 2800 3025
Wire Wire Line
	2800 3025 2800 3000
Text Notes 775  2225 0    50   Italic 10
Current limiting reference voltage
Wire Wire Line
	2525 1050 3025 1050
Connection ~ 3025 1050
$Comp
L power:PWR_FLAG #FLG01
U 1 1 5EF90A9E
P 9050 1150
F 0 "#FLG01" H 9050 1225 50  0001 C CNN
F 1 "PWR_FLAG" V 9050 1278 50  0000 L CNN
F 2 "" H 9050 1150 50  0001 C CNN
F 3 "~" H 9050 1150 50  0001 C CNN
	1    9050 1150
	0    1    1    0   
$EndComp
$Comp
L power:PWR_FLAG #FLG02
U 1 1 5EF90FCA
P 9050 1350
F 0 "#FLG02" H 9050 1425 50  0001 C CNN
F 1 "PWR_FLAG" V 9050 1478 50  0000 L CNN
F 2 "" H 9050 1350 50  0001 C CNN
F 3 "~" H 9050 1350 50  0001 C CNN
	1    9050 1350
	0    1    1    0   
$EndComp
Text Notes 4450 1350 0    50   ~ 10
MOTOR OUTPUT
Text Notes 6775 3350 0    50   ~ 0
Op-amp gain of 30 ⇒\n   ADC input 0 - 3 V\n\nGain = 1 + Rf / Rg ⇒\n  Rf / Rg = 29\n\nRf = 330 kΩ\nRg = 12 kΩ // 220 kΩ
$Comp
L mini-mapper:Conn_01x02 J1
U 1 1 5EF91CF7
P 8600 1300
F 0 "J1" H 8518 975 50  0000 C CNN
F 1 "Conn_01x02" H 8518 1066 50  0000 C CNN
F 2 "" H 8600 1300 50  0001 C CNN
F 3 "~" H 8600 1300 50  0001 C CNN
	1    8600 1300
	-1   0    0    1   
$EndComp
$Comp
L mini-mapper:VM #PWR02
U 1 1 5EF924B9
P 9050 1050
F 0 "#PWR02" H 8850 900 50  0001 C CNN
F 1 "VM" H 9065 1223 50  0000 C CNN
F 2 "" H 9050 1050 50  0001 C CNN
F 3 "" H 9050 1050 50  0001 C CNN
	1    9050 1050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR05
U 1 1 5EF92821
P 9050 1400
F 0 "#PWR05" H 9050 1150 50  0001 C CNN
F 1 "GND" H 9055 1227 50  0000 C CNN
F 2 "" H 9050 1400 50  0001 C CNN
F 3 "" H 9050 1400 50  0001 C CNN
	1    9050 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	9050 1400 9050 1350
Wire Wire Line
	8800 1200 9050 1200
Wire Wire Line
	9050 1200 9050 1150
Connection ~ 9050 1150
Wire Wire Line
	9050 1150 9050 1050
Wire Wire Line
	3025 1050 3650 1050
Connection ~ 9050 1350
Wire Wire Line
	9050 1350 9050 1300
Wire Wire Line
	3650 2275 3650 2725
Text Notes 8425 750  0    50   ~ 10
MOTOR POWER INPUT (4.5 V)
Text Notes 1225 1375 0    50   ~ 10
CONTROL INPUT
Text Label 2850 2075 0    50   ~ 0
Vref
$Comp
L mini-mapper:TLV9051 U2
U 1 1 5EF98393
P 6225 2325
F 0 "U2" H 6225 2550 50  0000 C CNN
F 1 "TLV9051" H 6225 2075 50  0000 C CNN
F 2 "mini-mapper:SOT-23-5" H 6225 2325 50  0001 C CNN
F 3 "https://www.ti.com/lit/ds/symlink/tlv9052.pdf" H 6225 2325 50  0001 C CNN
F 4 "TLV9051IDBVR" H 6225 2325 50  0001 C CNN "MPN"
F 5 "https://www.ti.com/product/TLV9052" H 6225 2325 50  0001 C CNN "Link"
	1    6225 2325
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:R R8
U 1 1 5EF99706
P 5550 2850
F 0 "R8" V 5450 2900 50  0000 R CNN
F 1 "220K" V 5375 2950 50  0000 R CNN
F 2 "mini-mapper:R_0805" V 5480 2850 50  0001 C CNN
F 3 "~" H 5550 2850 50  0001 C CNN
	1    5550 2850
	0    -1   -1   0   
$EndComp
$Comp
L mini-mapper:R R7
U 1 1 5EF99DCA
P 6225 2750
F 0 "R7" V 6125 2800 50  0000 R CNN
F 1 "330K" V 6050 2850 50  0000 R CNN
F 2 "mini-mapper:R_0805" V 6155 2750 50  0001 C CNN
F 3 "~" H 6225 2750 50  0001 C CNN
	1    6225 2750
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6525 2325 6600 2325
Wire Wire Line
	5925 2425 5850 2425
Wire Wire Line
	5850 2425 5850 2750
$Comp
L mini-mapper:TLV9051 U2
U 2 1 5EFA26E1
P 8775 2700
F 0 "U2" H 8803 2746 50  0000 L CNN
F 1 "TLV9051" H 8803 2655 50  0000 L CNN
F 2 "mini-mapper:SOT-23-5" H 8775 2700 50  0001 C CNN
F 3 "https://www.ti.com/lit/ds/symlink/tlv9052.pdf" H 8775 2700 50  0001 C CNN
	2    8775 2700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR012
U 1 1 5EFA3D5E
P 8675 3075
F 0 "#PWR012" H 8675 2825 50  0001 C CNN
F 1 "GND" H 8680 2902 50  0000 C CNN
F 2 "" H 8675 3075 50  0001 C CNN
F 3 "" H 8675 3075 50  0001 C CNN
	1    8675 3075
	1    0    0    -1  
$EndComp
Wire Wire Line
	8675 2325 8675 2400
Wire Wire Line
	8675 3000 8675 3075
Text Notes 6175 3150 0    50   ~ 0
Rf
Text Notes 5500 3150 0    50   ~ 0
Rg
$Comp
L mini-mapper:R R5
U 1 1 5EFAA4AA
P 5550 2650
F 0 "R5" V 5650 2700 50  0000 R CNN
F 1 "12K" V 5725 2725 50  0000 R CNN
F 2 "mini-mapper:R_0805" V 5480 2650 50  0001 C CNN
F 3 "~" H 5550 2650 50  0001 C CNN
	1    5550 2650
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5750 2750 5750 2850
Wire Wire Line
	5750 2850 5700 2850
Wire Wire Line
	5850 2750 5750 2750
Wire Wire Line
	5700 2650 5750 2650
Wire Wire Line
	5750 2650 5750 2750
Connection ~ 5750 2750
Wire Wire Line
	5400 2650 5325 2650
Wire Wire Line
	5325 2850 5400 2850
$Comp
L mini-mapper:Conn_01x01 J4
U 1 1 5EFC82DA
P 7225 2325
F 0 "J4" H 7305 2367 50  0000 L CNN
F 1 "Conn_01x01" H 7305 2276 50  0000 L CNN
F 2 "" H 7225 2325 50  0001 C CNN
F 3 "~" H 7225 2325 50  0001 C CNN
	1    7225 2325
	1    0    0    -1  
$EndComp
Text Notes 6950 2150 0    50   ~ 10
CURRENT SENSE OUTPUT
Text Label 2050 1725 0    50   ~ 0
IN1
Text Label 2050 1825 0    50   ~ 0
IN2
Text Label 6725 2325 0    50   ~ 0
Vsense
Text Notes 3275 3700 0    50   ~ 0
Current: 0 ≤ I ≤ 250 mA\nKeep Vsense ≤ 100 mV\n\nRsense = 0.4 Ω ⇒ 0 ≤ Vsense ≤ 0.1 V\n\n(Pmax = 25 mW)\n
Text Notes 775  2950 0    50   ~ 0
Iout(max) = 0.1 Vref / Rsense\n\nWant Iout(max) = 250 mA\nwith VM = 4.5 V\n\nSetting Rsense = 0.4 Ω\n\nVref = 10 x 0.25 x 0.4 = 1 V\n
Wire Wire Line
	6600 2750 6375 2750
Wire Wire Line
	6600 2325 6600 2750
Wire Wire Line
	6075 2750 5850 2750
Connection ~ 5850 2750
Wire Wire Line
	4175 2675 4300 2550
Wire Wire Line
	4175 2300 4300 2425
Wire Notes Line
	4725 2400 4825 2400
Wire Notes Line
	4825 2400 4825 2575
Wire Notes Line
	4825 2575 4725 2575
Wire Notes Line
	4725 2575 4725 2400
Wire Notes Line
	4775 2575 4775 2750
Text Notes 4625 2900 0    50   ~ 0
Route as\ndiff. pair
Wire Wire Line
	5325 2650 5325 2750
Wire Wire Line
	5200 2750 5325 2750
Connection ~ 5325 2750
Wire Wire Line
	5325 2750 5325 2850
Wire Wire Line
	5200 2225 5925 2225
$Comp
L mini-mapper:Conn_01x02 J3
U 1 1 5F085051
P 4450 1825
F 0 "J3" H 4368 1500 50  0000 C CNN
F 1 "Conn_01x02" H 4368 1591 50  0000 C CNN
F 2 "" H 4450 1825 50  0001 C CNN
F 3 "~" H 4450 1825 50  0001 C CNN
	1    4450 1825
	1    0    0    1   
$EndComp
$Comp
L mini-mapper:Conn_01x02 #J1
U 1 1 5F0853E0
P 4675 1825
F 0 "#J1" H 4675 1950 50  0000 C CNN
F 1 "Conn_01x02" H 4675 2025 50  0000 C CNN
F 2 "" H 4675 1825 50  0001 C CNN
F 3 "~" H 4675 1825 50  0001 C CNN
	1    4675 1825
	-1   0    0    1   
$EndComp
Wire Wire Line
	4250 1725 4050 1725
Wire Wire Line
	4050 1825 4250 1825
Wire Wire Line
	4875 1725 4950 1725
Wire Wire Line
	4950 1725 4950 1450
Wire Wire Line
	4950 1450 5200 1450
Wire Wire Line
	4875 1825 4950 1825
Wire Wire Line
	4950 1825 4950 2050
Wire Wire Line
	4950 2050 5200 2050
Wire Wire Line
	6600 2325 7025 2325
Connection ~ 6600 2325
Text Notes 6750 2675 0    50   Italic 10
Current sense amplifier
$Comp
L Device:Net-Tie_2 NT3
U 1 1 5F094D37
P 4075 2300
F 0 "NT3" H 4075 2425 50  0000 C CNN
F 1 "Net-Tie_2" H 4150 2350 50  0000 C CNN
F 2 "" H 4075 2300 50  0001 C CNN
F 3 "~" H 4075 2300 50  0001 C CNN
	1    4075 2300
	1    0    0    -1  
$EndComp
$Comp
L Device:Net-Tie_2 NT4
U 1 1 5F09CC8D
P 4075 2675
F 0 "NT4" H 4075 2550 50  0000 C CNN
F 1 "Net-Tie_2" H 4150 2625 50  0000 C CNN
F 2 "" H 4075 2675 50  0001 C CNN
F 3 "~" H 4075 2675 50  0001 C CNN
	1    4075 2675
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 2275 3850 2300
Wire Wire Line
	3975 2300 3850 2300
Connection ~ 3850 2300
Wire Wire Line
	3850 2300 3850 2350
Wire Wire Line
	3850 2650 3850 2675
Wire Wire Line
	3975 2675 3850 2675
Connection ~ 3850 2675
Wire Wire Line
	3850 2675 3850 2725
Text Label 4350 2425 0    50   ~ 0
Isense+
Text Label 4350 2550 0    50   ~ 0
Isense-
Wire Wire Line
	4300 2425 5000 2425
Wire Wire Line
	5000 2425 5200 2225
Wire Wire Line
	4300 2550 5000 2550
Wire Wire Line
	5000 2550 5200 2750
Text Notes 12025 2300 0    100  Italic 20
QUESTIONS
Text Notes 12075 2600 0    50   ~ 0
1. Does the current sense amplifier need input filters?\n\n2. Does the current sense amplifier need compensation?
Wire Notes Line
	12000 2100 14375 2100
Wire Notes Line
	14375 2100 14375 3250
Wire Notes Line
	14375 3250 12000 3250
Wire Notes Line
	12000 3250 12000 2100
$Comp
L mini-mapper:R R1
U 1 1 5F0AAD95
P 13125 1300
F 0 "R1" V 12918 1300 50  0000 C CNN
F 1 "1K" V 13009 1300 50  0000 C CNN
F 2 "mini-mapper:R_0805" V 13055 1300 50  0001 C CNN
F 3 "~" H 13125 1300 50  0001 C CNN
	1    13125 1300
	0    1    1    0   
$EndComp
$Comp
L mini-mapper:R R2
U 1 1 5F0AB1CF
P 13125 1750
F 0 "R2" V 12918 1750 50  0000 C CNN
F 1 "1K" V 13009 1750 50  0000 C CNN
F 2 "mini-mapper:R_0805" V 13055 1750 50  0001 C CNN
F 3 "~" H 13125 1750 50  0001 C CNN
	1    13125 1750
	0    1    1    0   
$EndComp
$Comp
L Device:Net-Tie_2 NT1
U 1 1 5F0AB9A9
P 12600 1300
F 0 "NT1" H 12600 1481 50  0000 C CNN
F 1 "Net-Tie_2" H 12600 1390 50  0000 C CNN
F 2 "" H 12600 1300 50  0001 C CNN
F 3 "~" H 12600 1300 50  0001 C CNN
	1    12600 1300
	1    0    0    -1  
$EndComp
$Comp
L Device:Net-Tie_2 NT2
U 1 1 5F0ABBFC
P 12625 1750
F 0 "NT2" H 12625 1931 50  0000 C CNN
F 1 "Net-Tie_2" H 12625 1840 50  0000 C CNN
F 2 "" H 12625 1750 50  0001 C CNN
F 3 "~" H 12625 1750 50  0001 C CNN
	1    12625 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	12725 1750 12975 1750
Wire Wire Line
	12700 1300 12975 1300
$Comp
L mini-mapper:C C3
U 1 1 5F0AF3FA
P 13550 1525
F 0 "C3" H 13665 1571 50  0000 L CNN
F 1 "100p" H 13665 1480 50  0000 L CNN
F 2 "mini-mapper:C_0805" H 13588 1375 50  0001 C CNN
F 3 "~" H 13550 1525 50  0001 C CNN
	1    13550 1525
	1    0    0    -1  
$EndComp
Wire Wire Line
	13275 1300 13550 1300
Wire Wire Line
	13550 1300 13550 1375
Wire Wire Line
	13550 1750 13275 1750
Wire Wire Line
	13550 1675 13550 1750
Wire Wire Line
	13550 1300 14025 1300
Connection ~ 13550 1300
Wire Wire Line
	13550 1750 14025 1750
Connection ~ 13550 1750
Wire Wire Line
	12500 1300 12175 1300
Wire Wire Line
	12525 1750 12175 1750
Text Notes 12000 1625 0    50   ~ 0
Across\nsense\nresistor
Text Notes 13975 1650 0    50   ~ 0
To\nsense\namplifier
Text Notes 12025 925  0    50   ~ 0
Example current sense amplifier input filter\n(from TI SLVA959A)
Wire Wire Line
	9050 1300 8800 1300
Text Notes 625  850  0    150  ~ 30
MOTOR DRIVER
Text Notes 625  4525 0    150  ~ 30
MOTOR ENCODER
Wire Wire Line
	1925 5650 2025 5650
$Comp
L mini-mapper:R R11
U 1 1 5F1304D4
P 2025 6425
F 0 "R11" H 2095 6471 50  0000 L CNN
F 1 "CALC" H 2095 6380 50  0000 L CNN
F 2 "mini-mapper:R_0805" V 1955 6425 50  0001 C CNN
F 3 "~" H 2025 6425 50  0001 C CNN
	1    2025 6425
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR015
U 1 1 5F132CD7
P 2025 6725
F 0 "#PWR015" H 2025 6475 50  0001 C CNN
F 1 "GND" H 2030 6552 50  0000 C CNN
F 2 "" H 2025 6725 50  0001 C CNN
F 3 "" H 2025 6725 50  0001 C CNN
	1    2025 6725
	1    0    0    -1  
$EndComp
Wire Wire Line
	2025 6725 2025 6575
Wire Wire Line
	1025 5075 1025 5200
Wire Wire Line
	1025 5650 1125 5650
$Comp
L mini-mapper:R R10
U 1 1 5F13BFDF
P 1025 6425
F 0 "R10" H 1095 6471 50  0000 L CNN
F 1 "100" H 1095 6380 50  0000 L CNN
F 2 "mini-mapper:R_0805" V 955 6425 50  0001 C CNN
F 3 "~" H 1025 6425 50  0001 C CNN
	1    1025 6425
	1    0    0    -1  
$EndComp
Wire Wire Line
	1125 5950 1025 5950
Wire Wire Line
	1025 5950 1025 6275
$Comp
L power:GND #PWR014
U 1 1 5F13FC3D
P 1025 6700
F 0 "#PWR014" H 1025 6450 50  0001 C CNN
F 1 "GND" H 1030 6527 50  0000 C CNN
F 2 "" H 1025 6700 50  0001 C CNN
F 3 "" H 1025 6700 50  0001 C CNN
	1    1025 6700
	1    0    0    -1  
$EndComp
Wire Wire Line
	1025 6700 1025 6575
Wire Wire Line
	1925 5950 2025 5950
Wire Wire Line
	2025 6275 2025 5950
$Comp
L mini-mapper:R R9
U 1 1 5F14C8A5
P 2775 5700
F 0 "R9" H 2845 5746 50  0000 L CNN
F 1 "1K" H 2845 5655 50  0000 L CNN
F 2 "mini-mapper:R_0805" V 2705 5700 50  0001 C CNN
F 3 "~" H 2775 5700 50  0001 C CNN
	1    2775 5700
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:R R12
U 1 1 5F14D17A
P 2775 6425
F 0 "R12" H 2845 6471 50  0000 L CNN
F 1 "1K" H 2845 6380 50  0000 L CNN
F 2 "mini-mapper:R_0805" V 2705 6425 50  0001 C CNN
F 3 "~" H 2775 6425 50  0001 C CNN
	1    2775 6425
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR016
U 1 1 5F14D78C
P 2775 6725
F 0 "#PWR016" H 2775 6475 50  0001 C CNN
F 1 "GND" H 2780 6552 50  0000 C CNN
F 2 "" H 2775 6725 50  0001 C CNN
F 3 "" H 2775 6725 50  0001 C CNN
	1    2775 6725
	1    0    0    -1  
$EndComp
Wire Wire Line
	2775 5850 2775 6150
Wire Wire Line
	2775 6575 2775 6725
Connection ~ 2775 6150
Wire Wire Line
	2775 6150 2775 6275
Wire Wire Line
	2775 5550 2775 5200
Connection ~ 1025 5200
Wire Wire Line
	1025 5200 1025 5650
Wire Wire Line
	2025 5650 2025 5200
Wire Wire Line
	2025 5200 1025 5200
Wire Wire Line
	4275 6050 4400 6050
$Comp
L mini-mapper:Conn_01x01 J5
U 1 1 5F168F33
P 4925 6050
F 0 "J5" H 5005 6092 50  0000 L CNN
F 1 "Conn_01x01" H 5005 6001 50  0000 L CNN
F 2 "" H 4925 6050 50  0001 C CNN
F 3 "~" H 4925 6050 50  0001 C CNN
	1    4925 6050
	1    0    0    -1  
$EndComp
Text Notes 4625 5900 0    50   ~ 10
ENCODER PULSE OUTPUT
$Comp
L mini-mapper:TCST1202 U4
U 1 1 5F1365AB
P 1525 5850
F 0 "U4" H 1525 6275 50  0000 C CNN
F 1 "TCST1202" H 1525 6184 50  0000 C CNN
F 2 "" H 1525 5850 50  0001 C CNN
F 3 "" H 1525 5850 50  0001 C CNN
	1    1525 5850
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR013
U 1 1 5F13AB1F
P 1025 5075
F 0 "#PWR013" H 1025 4925 50  0001 C CNN
F 1 "+3V3" H 1040 5248 50  0000 C CNN
F 2 "" H 1025 5075 50  0001 C CNN
F 3 "" H 1025 5075 50  0001 C CNN
	1    1025 5075
	1    0    0    -1  
$EndComp
Text Notes 6000 5150 0    50   ~ 0
Vf = 1.25 V (typ.), 1.6 V (max.)\nIf = 20 mA (nominal from datasheet plots)\n\nR10 = 100Ω ⇒ If = 20.5 mA (typ.), 17 mA (min.)
$Comp
L power:+3V3 #PWR07
U 1 1 5F14C9FA
P 8675 2325
F 0 "#PWR07" H 8675 2175 50  0001 C CNN
F 1 "+3V3" H 8690 2498 50  0000 C CNN
F 2 "" H 8675 2325 50  0001 C CNN
F 3 "" H 8675 2325 50  0001 C CNN
	1    8675 2325
	1    0    0    -1  
$EndComp
Text Notes 8425 1950 0    50   ~ 10
3.3 V POWER FROM NUCLEO BOARD
Text Notes 8500 3575 0    50   ~ 0
Motor coil\ncurrent sense\nopamp
Text Notes 9525 3500 0    50   ~ 0
Motor encoder\ncomparator
Wire Wire Line
	3350 5950 3525 5950
Wire Wire Line
	2775 6150 3675 6150
$Comp
L mini-mapper:R R13
U 1 1 5F1773A4
P 3950 5425
F 0 "R13" V 3775 5375 50  0000 L CNN
F 1 "32K" V 3850 5350 50  0000 L CNN
F 2 "mini-mapper:R_0805" V 3880 5425 50  0001 C CNN
F 3 "~" H 3950 5425 50  0001 C CNN
	1    3950 5425
	0    1    1    0   
$EndComp
Wire Wire Line
	4100 5425 4400 5425
Wire Wire Line
	4400 5425 4400 6050
Connection ~ 4400 6050
Wire Wire Line
	4400 6050 4725 6050
Wire Wire Line
	3800 5425 3525 5425
Wire Wire Line
	3525 5425 3525 5950
Connection ~ 3525 5950
Wire Wire Line
	3525 5950 3675 5950
Connection ~ 2025 5950
Wire Wire Line
	2025 5200 2775 5200
Connection ~ 2025 5200
Wire Wire Line
	2025 5950 3050 5950
$Comp
L mini-mapper:R R14
U 1 1 5F17001A
P 3200 5950
F 0 "R14" V 3025 5900 50  0000 L CNN
F 1 "1K" V 3100 5875 50  0000 L CNN
F 2 "mini-mapper:R_0805" V 3130 5950 50  0001 C CNN
F 3 "~" H 3200 5950 50  0001 C CNN
	1    3200 5950
	0    1    1    0   
$EndComp
Text Notes 6000 5550 0    50   ~ 0
I(dark) = 100 nA (max.)\nI(light) = 1 mA (min.), 2 mA (typ.)
Text Notes 6000 4800 0    50   Italic 10
Infra-red LED
Text Notes 6000 5350 0    50   Italic 10
Phototransistor current
Text Label 2225 5950 0    50   ~ 0
Vphoto
Text Notes 6000 6200 0    50   ~ 0
To get Vphoto = 3.3 V for I(light) = 1 mA,\nput R11 = 3.3 kΩ\n\n(If I(light) > 1 mA, the phototransistor\njust goes into saturation anyway.)
Text Notes 6000 5750 0    50   Italic 10
Phototransistor current conversion
Text Notes 8325 5325 0    50   Italic 10
Comparator hysteresis
Text Notes 8325 5675 0    50   ~ 0
With comparator outputs 0 and 3.3 V,\nR14 = 1 kΩ, R13 = 32 kΩ gives\n\nV(hyst) = 1K/(1K + 32K) * (3.3 - 0) = 0.1 V
Text Notes 8325 4800 0    50   Italic 10
Comparator threshold
Text Notes 8325 5150 0    50   ~ 0
R9 and R12 set the comparator threshold\nat Vcc/2 = 1.65 V. This is large enough to\navoid problems with ambient light "dark\ncurrent" from the phototransistor.
Wire Notes Line
	475  4025 11225 4025
Wire Notes Line
	8175 475  8175 4025
$Comp
L mini-mapper:LMV331 U3
U 1 1 5F1EDF5B
P 3875 6050
F 0 "U3" H 3975 6415 50  0000 C CNN
F 1 "LMV331" H 3975 6324 50  0000 C CNN
F 2 "" H 3975 6150 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lmv331.pdf" H 3975 6250 50  0001 C CNN
	1    3875 6050
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:LMV331 U3
U 2 1 5F1EE91F
P 9750 2700
F 0 "U3" H 9828 2746 50  0000 L CNN
F 1 "LMV331" H 9828 2655 50  0000 L CNN
F 2 "" H 9850 2800 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lmv331.pdf" H 9850 2900 50  0001 C CNN
	2    9750 2700
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR09
U 1 1 5F1F2C41
P 9750 2300
F 0 "#PWR09" H 9750 2150 50  0001 C CNN
F 1 "+3V3" H 9765 2473 50  0000 C CNN
F 2 "" H 9750 2300 50  0001 C CNN
F 3 "" H 9750 2300 50  0001 C CNN
	1    9750 2300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR011
U 1 1 5F1F32D3
P 9750 3075
F 0 "#PWR011" H 9750 2825 50  0001 C CNN
F 1 "GND" H 9755 2902 50  0000 C CNN
F 2 "" H 9750 3075 50  0001 C CNN
F 3 "" H 9750 3075 50  0001 C CNN
	1    9750 3075
	1    0    0    -1  
$EndComp
Wire Wire Line
	9750 2300 9750 2375
Wire Wire Line
	9750 3000 9750 3025
$Comp
L mini-mapper:C C5
U 1 1 5F1FB2DE
P 9250 2700
F 0 "C5" H 9365 2746 50  0000 L CNN
F 1 "0.1U" H 9365 2655 50  0000 L CNN
F 2 "mini-mapper:C_0805" H 9288 2550 50  0001 C CNN
F 3 "~" H 9250 2700 50  0001 C CNN
	1    9250 2700
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:C C6
U 1 1 5F1FB700
P 10250 2700
F 0 "C6" H 10365 2746 50  0000 L CNN
F 1 "0.1U" H 10365 2655 50  0000 L CNN
F 2 "mini-mapper:C_0805" H 10288 2550 50  0001 C CNN
F 3 "~" H 10250 2700 50  0001 C CNN
	1    10250 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	10250 2375 9750 2375
Wire Wire Line
	10250 2375 10250 2550
Connection ~ 9750 2375
Wire Wire Line
	9750 2375 9750 2400
Wire Wire Line
	10250 2850 10250 3025
Wire Wire Line
	10250 3025 9750 3025
Connection ~ 9750 3025
Wire Wire Line
	9750 3025 9750 3075
Wire Wire Line
	9250 2550 9250 2400
Wire Wire Line
	9250 2400 8675 2400
Connection ~ 8675 2400
Wire Wire Line
	9250 3000 8675 3000
Connection ~ 8675 3000
Wire Wire Line
	9250 2850 9250 3000
$EndSCHEMATC
