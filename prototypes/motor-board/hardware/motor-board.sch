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
P 4000 3350
F 0 "U1" H 4200 3825 50  0000 C CNN
F 1 "TB67H450FNG" H 4000 3500 50  0000 C CNN
F 2 "mini-mapper:SOIC127P600X175-9N" H 4100 3300 50  0001 C CNN
F 3 "https://www.mouser.com/pdfDocs/TB67H450FNG_datasheet_en_20190401.pdf" H 4100 3300 50  0001 C CNN
F 4 "TB67H450FNG" H 4000 3350 50  0001 C CNN "MPN"
F 5 "https://toshiba.semicon-storage.com/us/semiconductor/product/motor-driver-ics/brushed-dc-motor-driver-ics/detail.TB67H450FNG.html" H 4000 3350 50  0001 C CNN "Link"
	1    4000 3350
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:Motor_DC #M1
U 1 1 5EF670DE
P 5550 3325
F 0 "#M1" H 5697 3321 50  0000 L CNN
F 1 "Motor_DC" H 5697 3230 50  0000 L CNN
F 2 "" H 5550 3235 50  0001 C CNN
F 3 "~" H 5550 3235 50  0001 C CNN
	1    5550 3325
	1    0    0    -1  
$EndComp
Wire Wire Line
	5550 3075 5550 3125
Wire Wire Line
	5550 3675 5550 3625
$Comp
L power:GND #PWR05
U 1 1 5EF680BB
P 4000 4400
F 0 "#PWR05" H 4000 4150 50  0001 C CNN
F 1 "GND" H 4005 4227 50  0000 C CNN
F 2 "" H 4000 4400 50  0001 C CNN
F 3 "" H 4000 4400 50  0001 C CNN
	1    4000 4400
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:R R2
U 1 1 5EF6879B
P 4200 4125
F 0 "R2" H 4270 4171 50  0000 L CNN
F 1 "400m" H 4270 4080 50  0000 L CNN
F 2 "mini-mapper:R_0805" V 4130 4125 50  0001 C CNN
F 3 "~" H 4200 4125 50  0001 C CNN
	1    4200 4125
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 4350 4000 4350
Connection ~ 4000 4350
Wire Wire Line
	4000 4350 4000 4400
Text Notes 3625 4800 0    50   Italic 10
Current sense resistor
$Comp
L mini-mapper:VM #PWR01
U 1 1 5EF69E1B
P 4000 2625
F 0 "#PWR01" H 3800 2475 50  0001 C CNN
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
L mini-mapper:C C2
U 1 1 5EF81780
P 3375 2875
F 0 "C2" H 3490 2921 50  0000 L CNN
F 1 "0.1U" H 3490 2830 50  0000 L CNN
F 2 "mini-mapper:C_0805" H 3413 2725 50  0001 C CNN
F 3 "~" H 3375 2875 50  0001 C CNN
	1    3375 2875
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:C C3
U 1 1 5EF81A93
P 3150 4475
F 0 "C3" H 3265 4521 50  0000 L CNN
F 1 "0.1U" H 3265 4430 50  0000 L CNN
F 2 "mini-mapper:C_0805" H 3188 4325 50  0001 C CNN
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
L mini-mapper:R R4
U 1 1 5EF821E0
P 2750 4375
F 0 "R4" H 2820 4421 50  0000 L CNN
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
L power:GND #PWR03
U 1 1 5EF832CB
P 3375 3075
F 0 "#PWR03" H 3375 2825 50  0001 C CNN
F 1 "GND" H 3380 2902 50  0000 C CNN
F 2 "" H 3375 3075 50  0001 C CNN
F 3 "" H 3375 3075 50  0001 C CNN
	1    3375 3075
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 5EF83951
P 2875 3075
F 0 "#PWR02" H 2875 2825 50  0001 C CNN
F 1 "GND" H 2880 2902 50  0000 C CNN
F 2 "" H 2875 3075 50  0001 C CNN
F 3 "" H 2875 3075 50  0001 C CNN
	1    2875 3075
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR06
U 1 1 5EF83D45
P 3150 4700
F 0 "#PWR06" H 3150 4450 50  0001 C CNN
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
L mini-mapper:VM #PWR04
U 1 1 5EF87B52
P 2750 3775
F 0 "#PWR04" H 2550 3625 50  0001 C CNN
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
Text Notes 4800 2975 0    50   ~ 10
MOTOR OUTPUT
Text Notes 5775 5650 0    50   ~ 0
Op-amp gain of 30 ⇒\n   ADC input 0 - 3 V\n\nGain = 1 + Rf / Rg ⇒\n  Rf / Rg = 29\n\nRf = 330 kΩ\nRg = 12 kΩ // 220 kΩ
$Comp
L mini-mapper:Conn_01x02 J4
U 1 1 5EF91CF7
P 1800 6300
F 0 "J4" H 1718 5975 50  0000 C CNN
F 1 "Conn_01x02" H 1718 6066 50  0000 C CNN
F 2 "" H 1800 6300 50  0001 C CNN
F 3 "~" H 1800 6300 50  0001 C CNN
	1    1800 6300
	-1   0    0    1   
$EndComp
$Comp
L mini-mapper:VM #PWR08
U 1 1 5EF924B9
P 2375 6050
F 0 "#PWR08" H 2175 5900 50  0001 C CNN
F 1 "VM" H 2390 6223 50  0000 C CNN
F 2 "" H 2375 6050 50  0001 C CNN
F 3 "" H 2375 6050 50  0001 C CNN
	1    2375 6050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR09
U 1 1 5EF92821
P 2375 6400
F 0 "#PWR09" H 2375 6150 50  0001 C CNN
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
L mini-mapper:TLV9051 U2
U 1 1 5EF98393
P 6575 3950
F 0 "U2" H 6575 4175 50  0000 C CNN
F 1 "TLV9051" H 6575 3700 50  0000 C CNN
F 2 "mini-mapper:SOT-23-5" H 6575 3950 50  0001 C CNN
F 3 "https://www.ti.com/lit/ds/symlink/tlv9052.pdf" H 6575 3950 50  0001 C CNN
F 4 "TLV9051IDBVR" H 6575 3950 50  0001 C CNN "MPN"
F 5 "https://www.ti.com/product/TLV9052" H 6575 3950 50  0001 C CNN "Link"
	1    6575 3950
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:R R6
U 1 1 5EF99706
P 5900 4475
F 0 "R6" V 5800 4525 50  0000 R CNN
F 1 "220K" V 5725 4575 50  0000 R CNN
F 2 "mini-mapper:R_0805" V 5830 4475 50  0001 C CNN
F 3 "~" H 5900 4475 50  0001 C CNN
	1    5900 4475
	0    -1   -1   0   
$EndComp
$Comp
L mini-mapper:R R5
U 1 1 5EF99DCA
P 6575 4375
F 0 "R5" V 6475 4425 50  0000 R CNN
F 1 "330K" V 6400 4475 50  0000 R CNN
F 2 "mini-mapper:R_0805" V 6505 4375 50  0001 C CNN
F 3 "~" H 6575 4375 50  0001 C CNN
	1    6575 4375
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6875 3950 6950 3950
Wire Wire Line
	6275 4050 6200 4050
Wire Wire Line
	6200 4050 6200 4375
$Comp
L mini-mapper:TLV9051 U2
U 2 1 5EFA26E1
P 3775 6200
F 0 "U2" H 3803 6246 50  0000 L CNN
F 1 "TLV9051" H 3803 6155 50  0000 L CNN
F 2 "mini-mapper:SOT-23-5" H 3775 6200 50  0001 C CNN
F 3 "https://www.ti.com/lit/ds/symlink/tlv9052.pdf" H 3775 6200 50  0001 C CNN
	2    3775 6200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR010
U 1 1 5EFA3D5E
P 3675 6575
F 0 "#PWR010" H 3675 6325 50  0001 C CNN
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
Text Notes 6525 4775 0    50   ~ 0
Rf
Text Notes 5850 4775 0    50   ~ 0
Rg
$Comp
L mini-mapper:R R3
U 1 1 5EFAA4AA
P 5900 4275
F 0 "R3" V 6000 4325 50  0000 R CNN
F 1 "12K" V 6075 4350 50  0000 R CNN
F 2 "mini-mapper:R_0805" V 5830 4275 50  0001 C CNN
F 3 "~" H 5900 4275 50  0001 C CNN
	1    5900 4275
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6100 4375 6100 4475
Wire Wire Line
	6100 4475 6050 4475
Wire Wire Line
	6200 4375 6100 4375
Wire Wire Line
	6050 4275 6100 4275
Wire Wire Line
	6100 4275 6100 4375
Connection ~ 6100 4375
Wire Wire Line
	5750 4275 5675 4275
Wire Wire Line
	5675 4475 5750 4475
$Comp
L mini-mapper:Conn_01x01 J3
U 1 1 5EFC82DA
P 7575 3950
F 0 "J3" H 7655 3992 50  0000 L CNN
F 1 "Conn_01x01" H 7655 3901 50  0000 L CNN
F 2 "" H 7575 3950 50  0001 C CNN
F 3 "~" H 7575 3950 50  0001 C CNN
	1    7575 3950
	1    0    0    -1  
$EndComp
Text Notes 7300 3775 0    50   ~ 10
CURRENT SENSE OUTPUT
Text Label 2400 3350 0    50   ~ 0
IN1
Text Label 2400 3450 0    50   ~ 0
IN2
Text Label 7075 3950 0    50   ~ 0
Vsense
Text Notes 3625 5400 0    50   ~ 0
Current: 0 ≤ I ≤ 250 mA\nKeep Vsense ≤ 100 mV\n\nRsense = 0.4 Ω ⇒\n   0 ≤ Vsense ≤ 0.1 V\n\n(Pmax = 25 mW)\n
Text Notes 1125 4575 0    50   ~ 0
Iout(max) = 0.1 Vref / Rsense\n\nWant Iout(max) = 250 mA\nwith VM = 4.5 V\n\nSetting Rsense = 0.4 Ω\n\nVref = 10 x 0.25 x 0.4 = 1 V\n
Wire Wire Line
	6950 4375 6725 4375
Wire Wire Line
	6950 3950 6950 4375
Wire Wire Line
	6425 4375 6200 4375
Connection ~ 6200 4375
Wire Wire Line
	4525 4300 4650 4175
Wire Wire Line
	4525 3925 4650 4050
Wire Notes Line
	5075 4025 5175 4025
Wire Notes Line
	5175 4025 5175 4200
Wire Notes Line
	5175 4200 5075 4200
Wire Notes Line
	5075 4200 5075 4025
Wire Notes Line
	5125 4200 5125 4375
Text Notes 4975 4525 0    50   ~ 0
Route as\ndiff. pair
Wire Wire Line
	5675 4275 5675 4375
Wire Wire Line
	5550 4375 5675 4375
Connection ~ 5675 4375
Wire Wire Line
	5675 4375 5675 4475
Wire Wire Line
	5550 3850 6275 3850
$Comp
L mini-mapper:Conn_01x02 J2
U 1 1 5F085051
P 4800 3450
F 0 "J2" H 4718 3125 50  0000 C CNN
F 1 "Conn_01x02" H 4718 3216 50  0000 C CNN
F 2 "" H 4800 3450 50  0001 C CNN
F 3 "~" H 4800 3450 50  0001 C CNN
	1    4800 3450
	1    0    0    1   
$EndComp
$Comp
L mini-mapper:Conn_01x02 #J1
U 1 1 5F0853E0
P 5025 3450
F 0 "#J1" H 5025 3575 50  0000 C CNN
F 1 "Conn_01x02" H 5025 3650 50  0000 C CNN
F 2 "" H 5025 3450 50  0001 C CNN
F 3 "~" H 5025 3450 50  0001 C CNN
	1    5025 3450
	-1   0    0    1   
$EndComp
Wire Wire Line
	4600 3350 4400 3350
Wire Wire Line
	4400 3450 4600 3450
Wire Wire Line
	5225 3350 5300 3350
Wire Wire Line
	5300 3350 5300 3075
Wire Wire Line
	5300 3075 5550 3075
Wire Wire Line
	5225 3450 5300 3450
Wire Wire Line
	5300 3450 5300 3675
Wire Wire Line
	5300 3675 5550 3675
Wire Wire Line
	6950 3950 7375 3950
Connection ~ 6950 3950
Text Notes 5775 4975 0    50   Italic 10
Current sense amplifier
$Comp
L Device:Net-Tie_2 NT1
U 1 1 5F094D37
P 4425 3925
F 0 "NT1" H 4425 4050 50  0000 C CNN
F 1 "Net-Tie_2" H 4500 3975 50  0000 C CNN
F 2 "" H 4425 3925 50  0001 C CNN
F 3 "~" H 4425 3925 50  0001 C CNN
	1    4425 3925
	1    0    0    -1  
$EndComp
$Comp
L Device:Net-Tie_2 NT2
U 1 1 5F09CC8D
P 4425 4300
F 0 "NT2" H 4425 4175 50  0000 C CNN
F 1 "Net-Tie_2" H 4500 4250 50  0000 C CNN
F 2 "" H 4425 4300 50  0001 C CNN
F 3 "~" H 4425 4300 50  0001 C CNN
	1    4425 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 3900 4200 3925
Wire Wire Line
	4325 3925 4200 3925
Connection ~ 4200 3925
Wire Wire Line
	4200 3925 4200 3975
Wire Wire Line
	4200 4275 4200 4300
Wire Wire Line
	4325 4300 4200 4300
Connection ~ 4200 4300
Wire Wire Line
	4200 4300 4200 4350
Text Label 4700 4050 0    50   ~ 0
Isense+
Text Label 4700 4175 0    50   ~ 0
Isense-
Wire Wire Line
	4650 4050 5350 4050
Wire Wire Line
	5350 4050 5550 3850
Wire Wire Line
	4650 4175 5350 4175
Wire Wire Line
	5350 4175 5550 4375
Text Notes 7775 4875 0    100  Italic 20
QUESTIONS
Text Notes 7775 5750 0    100  ~ 0
Does the current sense\namplifier need input filters?\n\nDoes the current sense\namplifier need compensation?
Wire Notes Line
	7750 4675 10125 4675
Wire Notes Line
	10125 4675 10125 5825
Wire Notes Line
	10125 5825 7750 5825
Wire Notes Line
	7750 5825 7750 4675
$Comp
L mini-mapper:R R?
U 1 1 5F0AAD95
P 6975 1575
F 0 "R?" V 6768 1575 50  0000 C CNN
F 1 "1K" V 6859 1575 50  0000 C CNN
F 2 "mini-mapper:R_0805" V 6905 1575 50  0001 C CNN
F 3 "~" H 6975 1575 50  0001 C CNN
	1    6975 1575
	0    1    1    0   
$EndComp
$Comp
L mini-mapper:R R?
U 1 1 5F0AB1CF
P 6975 2025
F 0 "R?" V 6768 2025 50  0000 C CNN
F 1 "1K" V 6859 2025 50  0000 C CNN
F 2 "mini-mapper:R_0805" V 6905 2025 50  0001 C CNN
F 3 "~" H 6975 2025 50  0001 C CNN
	1    6975 2025
	0    1    1    0   
$EndComp
$Comp
L Device:Net-Tie_2 NT?
U 1 1 5F0AB9A9
P 6450 1575
F 0 "NT?" H 6450 1756 50  0000 C CNN
F 1 "Net-Tie_2" H 6450 1665 50  0000 C CNN
F 2 "" H 6450 1575 50  0001 C CNN
F 3 "~" H 6450 1575 50  0001 C CNN
	1    6450 1575
	1    0    0    -1  
$EndComp
$Comp
L Device:Net-Tie_2 NT?
U 1 1 5F0ABBFC
P 6475 2025
F 0 "NT?" H 6475 2206 50  0000 C CNN
F 1 "Net-Tie_2" H 6475 2115 50  0000 C CNN
F 2 "" H 6475 2025 50  0001 C CNN
F 3 "~" H 6475 2025 50  0001 C CNN
	1    6475 2025
	1    0    0    -1  
$EndComp
Wire Wire Line
	6575 2025 6825 2025
Wire Wire Line
	6550 1575 6825 1575
$Comp
L mini-mapper:C C?
U 1 1 5F0AF3FA
P 7400 1800
F 0 "C?" H 7515 1846 50  0000 L CNN
F 1 "100p" H 7515 1755 50  0000 L CNN
F 2 "mini-mapper:C_0805" H 7438 1650 50  0001 C CNN
F 3 "~" H 7400 1800 50  0001 C CNN
	1    7400 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	7125 1575 7400 1575
Wire Wire Line
	7400 1575 7400 1650
Wire Wire Line
	7400 2025 7125 2025
Wire Wire Line
	7400 1950 7400 2025
Wire Wire Line
	7400 1575 7875 1575
Connection ~ 7400 1575
Wire Wire Line
	7400 2025 7875 2025
Connection ~ 7400 2025
Wire Wire Line
	6350 1575 6025 1575
Wire Wire Line
	6375 2025 6025 2025
Text Notes 5850 1900 0    50   ~ 0
Across\nsense\nresistor
Text Notes 7825 1925 0    50   ~ 0
To\nsense\namplifier
Text Notes 5875 1200 0    50   ~ 0
Example current sense amplifier input filter\n(from TI SLVA959A)
$EndSCHEMATC
