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
Comment3 "Motor driver and encoder prototype board"
Comment4 ""
$EndDescr
$Comp
L mini-mapper:TB67H450FNG U2
U 1 1 5EF667BD
P 3450 1750
F 0 "U2" H 3650 2225 50  0000 C CNN
F 1 "TB67H450FNG" H 3450 1900 50  0000 C CNN
F 2 "mini-mapper:SOIC127P600X175-9N" H 3550 1700 50  0001 C CNN
F 3 "https://www.mouser.com/pdfDocs/TB67H450FNG_datasheet_en_20190401.pdf" H 3550 1700 50  0001 C CNN
F 4 "TB67H450FNG" H 3450 1750 50  0001 C CNN "MPN"
F 5 "https://toshiba.semicon-storage.com/us/semiconductor/product/motor-driver-ics/brushed-dc-motor-driver-ics/detail.TB67H450FNG.html" H 3450 1750 50  0001 C CNN "Link"
	1    3450 1750
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:Motor_DC #M1
U 1 1 5EF670DE
P 5350 1725
F 0 "#M1" H 5497 1721 50  0000 L CNN
F 1 "Motor_DC" H 5497 1630 50  0000 L CNN
F 2 "" H 5350 1635 50  0001 C CNN
F 3 "~" H 5350 1635 50  0001 C CNN
	1    5350 1725
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 1475 5350 1525
Wire Wire Line
	5350 2075 5350 2025
$Comp
L power:GND #PWR011
U 1 1 5EF680BB
P 3450 2800
F 0 "#PWR011" H 3450 2550 50  0001 C CNN
F 1 "GND" H 3455 2627 50  0000 C CNN
F 2 "" H 3450 2800 50  0001 C CNN
F 3 "" H 3450 2800 50  0001 C CNN
	1    3450 2800
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:R R2
U 1 1 5EF6879B
P 3650 2525
F 0 "R2" H 3720 2571 50  0000 L CNN
F 1 "400m" H 3720 2480 50  0000 L CNN
F 2 "mini-mapper:R_0805" V 3580 2525 50  0001 C CNN
F 3 "https://www.mouser.at/datasheet/2/447/PYu-RL_Group_521_RoHS_L_2-1131815.pdf" H 3650 2525 50  0001 C CNN
F 4 "RL0805FR-070R4L" H 3650 2525 50  0001 C CNN "MPN"
	1    3650 2525
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 2750 3450 2750
Connection ~ 3450 2750
Wire Wire Line
	3450 2750 3450 2800
Text Notes 3075 3200 0    50   Italic 10
Current sense resistor
$Comp
L mini-mapper:VM #PWR01
U 1 1 5EF69E1B
P 3450 1025
F 0 "#PWR01" H 3250 875 50  0001 C CNN
F 1 "VM" H 3465 1198 50  0000 C CNN
F 2 "" H 3450 1025 50  0001 C CNN
F 3 "" H 3450 1025 50  0001 C CNN
	1    3450 1025
	1    0    0    -1  
$EndComp
Wire Wire Line
	3450 1025 3450 1075
$Comp
L mini-mapper:C C2
U 1 1 5EF81780
P 2825 1275
F 0 "C2" H 2940 1321 50  0000 L CNN
F 1 "0.1U" H 2940 1230 50  0000 L CNN
F 2 "mini-mapper:C_0805" H 2863 1125 50  0001 C CNN
F 3 "http://datasheet.octopart.com/885012207098-W%C3%BCrth-Elektronik-datasheet-137006532.pdf" H 2825 1275 50  0001 C CNN
F 4 "885012207098" H 2825 1275 50  0001 C CNN "MPN"
	1    2825 1275
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:C C5
U 1 1 5EF81A93
P 2600 2875
F 0 "C5" H 2715 2921 50  0000 L CNN
F 1 "0.1U" H 2715 2830 50  0000 L CNN
F 2 "mini-mapper:C_0805" H 2638 2725 50  0001 C CNN
F 3 "http://datasheet.octopart.com/885012207098-W%C3%BCrth-Elektronik-datasheet-137006532.pdf" H 2600 2875 50  0001 C CNN
F 4 "885012207098" H 2600 2875 50  0001 C CNN "MPN"
	1    2600 2875
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:R R1
U 1 1 5EF81CFE
P 2200 2375
F 0 "R1" H 2270 2421 50  0000 L CNN
F 1 "56K" H 2270 2330 50  0000 L CNN
F 2 "mini-mapper:R_0805" V 2130 2375 50  0001 C CNN
F 3 "http://datasheet.octopart.com/CRGP0805F56K-TE-Connectivity-datasheet-114718804.pdf" H 2200 2375 50  0001 C CNN
F 4 "CRGP0805F56K" H 2200 2375 50  0001 C CNN "MPN"
	1    2200 2375
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:R R4
U 1 1 5EF821E0
P 2200 2775
F 0 "R4" H 2270 2821 50  0000 L CNN
F 1 "16K" H 2270 2730 50  0000 L CNN
F 2 "mini-mapper:R_0805" V 2130 2775 50  0001 C CNN
F 3 "https://www.mouser.at/datasheet/2/427/dcrcwe3-1762152.pdf" H 2200 2775 50  0001 C CNN
F 4 "CRCW080516K0FKEA" H 2200 2775 50  0001 C CNN "MPN"
	1    2200 2775
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:CP C1
U 1 1 5EF82B68
P 2325 1275
F 0 "C1" H 2443 1321 50  0000 L CNN
F 1 "100U" H 2443 1230 50  0000 L CNN
F 2 "mini-mapper:CP_Elec_6.3x7.7" H 2363 1125 50  0001 C CNN
F 3 "https://www.mouser.at/datasheet/2/315/ABA0000C1160-947462.pdf" H 2325 1275 50  0001 C CNN
F 4 "EEE-HC1C101XP" H 2325 1275 50  0001 C CNN "MPN"
	1    2325 1275
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR03
U 1 1 5EF832CB
P 2825 1475
F 0 "#PWR03" H 2825 1225 50  0001 C CNN
F 1 "GND" H 2830 1302 50  0000 C CNN
F 2 "" H 2825 1475 50  0001 C CNN
F 3 "" H 2825 1475 50  0001 C CNN
	1    2825 1475
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 5EF83951
P 2325 1475
F 0 "#PWR02" H 2325 1225 50  0001 C CNN
F 1 "GND" H 2330 1302 50  0000 C CNN
F 2 "" H 2325 1475 50  0001 C CNN
F 3 "" H 2325 1475 50  0001 C CNN
	1    2325 1475
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR013
U 1 1 5EF83D45
P 2600 3100
F 0 "#PWR013" H 2600 2850 50  0001 C CNN
F 1 "GND" H 2605 2927 50  0000 C CNN
F 2 "" H 2600 3100 50  0001 C CNN
F 3 "" H 2600 3100 50  0001 C CNN
	1    2600 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 3100 2600 3050
Wire Wire Line
	2600 2725 2600 2575
Wire Wire Line
	2825 1475 2825 1425
Wire Wire Line
	2325 1475 2325 1425
Wire Wire Line
	2325 1125 2325 1075
Wire Wire Line
	2825 1125 2825 1075
Connection ~ 3450 1075
Wire Wire Line
	3450 1075 3450 1250
$Comp
L mini-mapper:VM #PWR09
U 1 1 5EF87B52
P 2200 2175
F 0 "#PWR09" H 2000 2025 50  0001 C CNN
F 1 "VM" H 2215 2348 50  0000 C CNN
F 2 "" H 2200 2175 50  0001 C CNN
F 3 "" H 2200 2175 50  0001 C CNN
	1    2200 2175
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 2175 2200 2225
Wire Wire Line
	2200 2525 2200 2575
Connection ~ 2200 2575
Wire Wire Line
	2200 2575 2200 2625
Wire Wire Line
	2200 2575 2600 2575
Wire Wire Line
	2600 2100 2600 2575
Connection ~ 2600 2575
Wire Wire Line
	2600 2100 3050 2100
Wire Wire Line
	2200 3050 2600 3050
Wire Wire Line
	2200 2925 2200 3050
Connection ~ 2600 3050
Wire Wire Line
	2600 3050 2600 3025
Text Notes 575  2250 0    50   Italic 10
Current limiting reference voltage
Wire Wire Line
	2325 1075 2825 1075
Connection ~ 2825 1075
$Comp
L power:PWR_FLAG #FLG02
U 1 1 5EF90FCA
P 8800 2025
F 0 "#FLG02" H 8800 2100 50  0001 C CNN
F 1 "PWR_FLAG" V 8800 2153 50  0000 L CNN
F 2 "" H 8800 2025 50  0001 C CNN
F 3 "~" H 8800 2025 50  0001 C CNN
	1    8800 2025
	0    -1   -1   0   
$EndComp
Text Notes 4250 1375 0    50   ~ 10
MOTOR OUTPUT
Text Notes 6575 3375 0    50   ~ 0
Op-amp gain of 30 ⇒\n   ADC input 0 - 3 V\n\nGain = 1 + Rf / Rg ⇒\n  Rf / Rg = 29\n\nRf = 330 kΩ\nRg = 12 kΩ // 220 kΩ
Wire Wire Line
	2825 1075 3450 1075
Wire Wire Line
	3450 2300 3450 2750
Text Notes 7975 1225 0    50   ~ 10
MOTOR POWER INPUT (5V to 4.5 V)
Text Notes 900  1450 0    50   ~ 10
CONTROL INPUT
Text Label 2650 2100 0    50   ~ 0
Vref
$Comp
L mini-mapper:TLV9051 U3
U 1 1 5EF98393
P 6025 2350
F 0 "U3" H 6025 2575 50  0000 C CNN
F 1 "TLV9051" H 6025 2100 50  0000 C CNN
F 2 "mini-mapper:SOT-23-5" H 6025 2350 50  0001 C CNN
F 3 "https://www.ti.com/lit/ds/symlink/tlv9052.pdf" H 6025 2350 50  0001 C CNN
F 4 "TLV9051IDBVR" H 6025 2350 50  0001 C CNN "MPN"
F 5 "https://www.ti.com/product/TLV9052" H 6025 2350 50  0001 C CNN "Link"
	1    6025 2350
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:R R6
U 1 1 5EF99706
P 5350 2875
F 0 "R6" V 5250 2925 50  0000 R CNN
F 1 "220K" V 5175 2975 50  0000 R CNN
F 2 "mini-mapper:R_0805" V 5280 2875 50  0001 C CNN
F 3 "http://datasheet.octopart.com/CRGP0805F220K-TE-Connectivity-datasheet-114718804.pdf" H 5350 2875 50  0001 C CNN
F 4 "CRGP0805F220K" H 5350 2875 50  0001 C CNN "MPN"
	1    5350 2875
	0    -1   -1   0   
$EndComp
$Comp
L mini-mapper:R R5
U 1 1 5EF99DCA
P 6025 2775
F 0 "R5" V 5925 2825 50  0000 R CNN
F 1 "330K" V 5850 2875 50  0000 R CNN
F 2 "mini-mapper:R_0805" V 5955 2775 50  0001 C CNN
F 3 "https://www.mouser.at/datasheet/2/427/dcrcwe3-1762152.pdf" H 6025 2775 50  0001 C CNN
F 4 "CRCW0805330KFKEA" H 6025 2775 50  0001 C CNN "MPN"
	1    6025 2775
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6325 2350 6400 2350
Wire Wire Line
	5725 2450 5650 2450
Wire Wire Line
	5650 2450 5650 2775
$Comp
L mini-mapper:TLV9051 U3
U 2 1 5EFA26E1
P 8625 3175
F 0 "U3" H 8653 3221 50  0000 L CNN
F 1 "TLV9051" H 8653 3130 50  0000 L CNN
F 2 "mini-mapper:SOT-23-5" H 8625 3175 50  0001 C CNN
F 3 "https://www.ti.com/lit/ds/symlink/tlv9052.pdf" H 8625 3175 50  0001 C CNN
F 4 "TLV9051IDBVR" H 8625 3175 50  0001 C CNN "MPN"
	2    8625 3175
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR014
U 1 1 5EFA3D5E
P 8525 3550
F 0 "#PWR014" H 8525 3300 50  0001 C CNN
F 1 "GND" H 8530 3377 50  0000 C CNN
F 2 "" H 8525 3550 50  0001 C CNN
F 3 "" H 8525 3550 50  0001 C CNN
	1    8525 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	8525 2800 8525 2875
Wire Wire Line
	8525 3475 8525 3550
Text Notes 5975 3175 0    50   ~ 0
Rf
Text Notes 5300 3175 0    50   ~ 0
Rg
$Comp
L mini-mapper:R R3
U 1 1 5EFAA4AA
P 5350 2675
F 0 "R3" V 5450 2725 50  0000 R CNN
F 1 "12K" V 5525 2750 50  0000 R CNN
F 2 "mini-mapper:R_0805" V 5280 2675 50  0001 C CNN
F 3 "https://www.mouser.at/datasheet/2/427/dcrcwe3-1762152.pdf" H 5350 2675 50  0001 C CNN
F 4 "CRCW080512K0FKEA" H 5350 2675 50  0001 C CNN "MPN"
	1    5350 2675
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5550 2775 5550 2875
Wire Wire Line
	5550 2875 5500 2875
Wire Wire Line
	5650 2775 5550 2775
Wire Wire Line
	5500 2675 5550 2675
Wire Wire Line
	5550 2675 5550 2775
Connection ~ 5550 2775
Wire Wire Line
	5200 2675 5125 2675
Wire Wire Line
	5125 2875 5200 2875
Text Notes 6575 2175 0    50   ~ 10
CURRENT SENSE OUTPUT
Text Label 1025 1750 0    50   ~ 0
MOTOR_IN1
Text Label 1025 1850 0    50   ~ 0
MOTOR_IN2
Text Label 6750 2350 0    50   ~ 0
MOTOR_SENSE
Text Notes 3075 3725 0    50   ~ 0
Current: 0 ≤ I ≤ 250 mA\nKeep Vsense ≤ 100 mV\n\nRsense = 0.4 Ω ⇒ 0 ≤ Vsense ≤ 0.1 V\n\n(Pmax = 25 mW)\n
Text Notes 575  2975 0    50   ~ 0
Iout(max) = 0.1 Vref / Rsense\n\nWant Iout(max) = 250 mA\nwith VM = 4.5 V\n\nSetting Rsense = 0.4 Ω\n\nVref = 10 x 0.25 x 0.4 = 1 V\n
Wire Wire Line
	6400 2775 6175 2775
Wire Wire Line
	6400 2350 6400 2775
Wire Wire Line
	5875 2775 5650 2775
Connection ~ 5650 2775
Wire Wire Line
	3975 2700 4100 2575
Wire Wire Line
	3975 2325 4100 2450
Wire Notes Line
	4525 2425 4625 2425
Wire Notes Line
	4625 2425 4625 2600
Wire Notes Line
	4625 2600 4525 2600
Wire Notes Line
	4525 2600 4525 2425
Wire Notes Line
	4575 2600 4575 2775
Text Notes 4425 2925 0    50   ~ 0
Route as\ndiff. pair
Wire Wire Line
	5125 2675 5125 2775
Wire Wire Line
	5000 2775 5125 2775
Connection ~ 5125 2775
Wire Wire Line
	5125 2775 5125 2875
Wire Wire Line
	5000 2250 5725 2250
$Comp
L mini-mapper:Conn_01x02 J2
U 1 1 5F085051
P 4600 1850
F 0 "J2" H 4518 1525 50  0000 C CNN
F 1 "Conn_01x02" H 4518 1616 50  0000 C CNN
F 2 "mini-mapper:PinSocket_1x02_P2.54mm_Vertical" H 4600 1850 50  0001 C CNN
F 3 "https://www.mouser.at/datasheet/2/527/slw-1370256.pdf" H 4600 1850 50  0001 C CNN
F 4 "SLW-102-01-T-S" H 4600 1850 50  0001 C CNN "MPN"
	1    4600 1850
	1    0    0    1   
$EndComp
$Comp
L mini-mapper:Conn_01x02 #J1
U 1 1 5F0853E0
P 4825 1850
F 0 "#J1" H 4825 1975 50  0000 C CNN
F 1 "Conn_01x02" H 4825 2050 50  0000 C CNN
F 2 "" H 4825 1850 50  0001 C CNN
F 3 "~" H 4825 1850 50  0001 C CNN
	1    4825 1850
	-1   0    0    1   
$EndComp
Wire Wire Line
	5025 1750 5100 1750
Wire Wire Line
	5100 1750 5100 1475
Wire Wire Line
	5100 1475 5350 1475
Wire Wire Line
	5025 1850 5100 1850
Wire Wire Line
	5100 1850 5100 2075
Wire Wire Line
	5100 2075 5350 2075
Connection ~ 6400 2350
Text Notes 6550 2700 0    50   Italic 10
Current sense amplifier
Wire Wire Line
	3650 2300 3650 2325
Connection ~ 3650 2325
Wire Wire Line
	3650 2325 3650 2375
Wire Wire Line
	3650 2675 3650 2700
Connection ~ 3650 2700
Wire Wire Line
	3650 2700 3650 2750
Text Label 4150 2450 0    50   ~ 0
Isense+
Text Label 4150 2575 0    50   ~ 0
Isense-
Wire Wire Line
	4100 2450 4800 2450
Wire Wire Line
	4800 2450 5000 2250
Wire Wire Line
	4100 2575 4800 2575
Wire Wire Line
	4800 2575 5000 2775
Text Notes 625  850  0    150  ~ 30
MOTOR DRIVER
Text Notes 625  4525 0    150  ~ 30
MOTOR ENCODER
Wire Wire Line
	1625 5475 1725 5475
$Comp
L mini-mapper:R R11
U 1 1 5F1304D4
P 1725 6250
F 0 "R11" H 1795 6296 50  0000 L CNN
F 1 "5.6K" H 1795 6205 50  0000 L CNN
F 2 "mini-mapper:R_0805" V 1655 6250 50  0001 C CNN
F 3 "https://www.mouser.at/datasheet/2/427/crcwce3-1762584.pdf" H 1725 6250 50  0001 C CNN
F 4 "CRCW08055K60FKEAC" H 1725 6250 50  0001 C CNN "MPN"
	1    1725 6250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR020
U 1 1 5F132CD7
P 1725 6550
F 0 "#PWR020" H 1725 6300 50  0001 C CNN
F 1 "GND" H 1730 6377 50  0000 C CNN
F 2 "" H 1725 6550 50  0001 C CNN
F 3 "" H 1725 6550 50  0001 C CNN
	1    1725 6550
	1    0    0    -1  
$EndComp
Wire Wire Line
	1725 6550 1725 6400
Wire Wire Line
	725  4900 725  5025
Wire Wire Line
	725  5475 825  5475
$Comp
L mini-mapper:R R10
U 1 1 5F13BFDF
P 725 6250
F 0 "R10" H 795 6296 50  0000 L CNN
F 1 "82" H 795 6205 50  0000 L CNN
F 2 "mini-mapper:R_0805" V 655 6250 50  0001 C CNN
F 3 "https://www.mouser.at/datasheet/2/427/dcrcwe3-1762152.pdf" H 725 6250 50  0001 C CNN
F 4 "CRCW080582R0FKEA" H 725 6250 50  0001 C CNN "MPN"
	1    725  6250
	1    0    0    -1  
$EndComp
Wire Wire Line
	825  5775 725  5775
Wire Wire Line
	725  5775 725  6100
$Comp
L power:GND #PWR019
U 1 1 5F13FC3D
P 725 6525
F 0 "#PWR019" H 725 6275 50  0001 C CNN
F 1 "GND" H 730 6352 50  0000 C CNN
F 2 "" H 725 6525 50  0001 C CNN
F 3 "" H 725 6525 50  0001 C CNN
	1    725  6525
	1    0    0    -1  
$EndComp
Wire Wire Line
	725  6525 725  6400
Wire Wire Line
	1625 5775 1725 5775
Wire Wire Line
	1725 6100 1725 5775
$Comp
L mini-mapper:R R8
U 1 1 5F14C8A5
P 2475 5525
F 0 "R8" H 2545 5571 50  0000 L CNN
F 1 "1K" H 2545 5480 50  0000 L CNN
F 2 "mini-mapper:R_0805" V 2405 5525 50  0001 C CNN
F 3 "http://datasheet.octopart.com/CRGP0805F1K0-TE-Connectivity-datasheet-114718804.pdf" H 2475 5525 50  0001 C CNN
F 4 "CRGP0805F1K0" H 2475 5525 50  0001 C CNN "MPN"
	1    2475 5525
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:R R12
U 1 1 5F14D17A
P 2475 6250
F 0 "R12" H 2545 6296 50  0000 L CNN
F 1 "1K" H 2545 6205 50  0000 L CNN
F 2 "mini-mapper:R_0805" V 2405 6250 50  0001 C CNN
F 3 "http://datasheet.octopart.com/CRGP0805F1K0-TE-Connectivity-datasheet-114718804.pdf" H 2475 6250 50  0001 C CNN
F 4 "CRGP0805F1K0" H 2475 6250 50  0001 C CNN "MPN"
	1    2475 6250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR021
U 1 1 5F14D78C
P 2475 6550
F 0 "#PWR021" H 2475 6300 50  0001 C CNN
F 1 "GND" H 2480 6377 50  0000 C CNN
F 2 "" H 2475 6550 50  0001 C CNN
F 3 "" H 2475 6550 50  0001 C CNN
	1    2475 6550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2475 5675 2475 5975
Wire Wire Line
	2475 6400 2475 6550
Connection ~ 2475 5975
Wire Wire Line
	2475 5975 2475 6100
Wire Wire Line
	2475 5375 2475 5025
Connection ~ 725  5025
Wire Wire Line
	725  5025 725  5475
Wire Wire Line
	1725 5475 1725 5025
Wire Wire Line
	1725 5025 725  5025
Wire Wire Line
	3975 5875 4100 5875
Text Notes 4425 5725 0    50   ~ 10
ENCODER PULSE OUTPUT
$Comp
L mini-mapper:TCST1202 U5
U 1 1 5F1365AB
P 1225 5675
F 0 "U5" H 1225 6100 50  0000 C CNN
F 1 "TCST1202" H 1225 6009 50  0000 C CNN
F 2 "mini-mapper:TCST1202" H 1225 5675 50  0001 C CNN
F 3 "https://www.vishay.com/docs/83764/tcst1103.pdf" H 1225 5675 50  0001 C CNN
F 4 "TCST1202" H 1225 5675 50  0001 C CNN "MPN"
	1    1225 5675
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR016
U 1 1 5F13AB1F
P 725 4900
F 0 "#PWR016" H 725 4750 50  0001 C CNN
F 1 "+3V3" H 740 5073 50  0000 C CNN
F 2 "" H 725 4900 50  0001 C CNN
F 3 "" H 725 4900 50  0001 C CNN
	1    725  4900
	1    0    0    -1  
$EndComp
Text Notes 675  7400 0    50   ~ 0
Vf = 1.25 V (typ.), 1.6 V (max.)\nIf = 20 mA (nominal from datasheet plots)\n\nR10 = 82Ω ⇒ If = 25 mA (typ.), 21 mA (min.)
$Comp
L power:+3V3 #PWR012
U 1 1 5F14C9FA
P 8525 2800
F 0 "#PWR012" H 8525 2650 50  0001 C CNN
F 1 "+3V3" H 8540 2973 50  0000 C CNN
F 2 "" H 8525 2800 50  0001 C CNN
F 3 "" H 8525 2800 50  0001 C CNN
	1    8525 2800
	1    0    0    -1  
$EndComp
Text Notes 8000 2525 0    50   ~ 10
3.3 V POWER FROM NUCLEO BOARD
Text Notes 8175 4075 0    50   ~ 0
Motor coil\ncurrent sense\nopamp
Text Notes 9225 3975 0    50   ~ 0
Motor encoder\ncomparator
Wire Wire Line
	3050 5775 3225 5775
Wire Wire Line
	2475 5975 3375 5975
$Comp
L mini-mapper:R R7
U 1 1 5F1773A4
P 3650 5250
F 0 "R7" V 3475 5200 50  0000 L CNN
F 1 "240K" V 3550 5150 50  0000 L CNN
F 2 "mini-mapper:R_0805" V 3580 5250 50  0001 C CNN
F 3 "https://www.mouser.at/datasheet/2/427/dcrcwe3-1762152.pdf" H 3650 5250 50  0001 C CNN
F 4 "CRCW0805240KFKEA" H 3650 5250 50  0001 C CNN "MPN"
	1    3650 5250
	0    1    1    0   
$EndComp
Wire Wire Line
	3800 5250 4100 5250
Wire Wire Line
	4100 5250 4100 5875
Connection ~ 4100 5875
Wire Wire Line
	3500 5250 3225 5250
Wire Wire Line
	3225 5250 3225 5775
Connection ~ 3225 5775
Wire Wire Line
	3225 5775 3375 5775
Connection ~ 1725 5775
Wire Wire Line
	1725 5025 2475 5025
Connection ~ 1725 5025
Wire Wire Line
	1725 5775 1900 5775
$Comp
L mini-mapper:R R9
U 1 1 5F17001A
P 2900 5775
F 0 "R9" V 2725 5725 50  0000 L CNN
F 1 "7.5K" V 2800 5675 50  0000 L CNN
F 2 "mini-mapper:R_0805" V 2830 5775 50  0001 C CNN
F 3 "https://www.mouser.at/datasheet/2/427/dcrcwe3-1762152.pdf" H 2900 5775 50  0001 C CNN
F 4 "CRCW08057K50FKEA" H 2900 5775 50  0001 C CNN "MPN"
	1    2900 5775
	0    1    1    0   
$EndComp
Text Notes 3050 6925 0    50   ~ 0
I(dark) = 100 nA (max.)\nI(light) = 1 mA (min.), 2 mA (typ.)\n\nBecause of geometry of overlap of holes\nin motor encoder disk with optical aperture,\nwe'll get 0.5 mA (min.), 1 mA (typ.)
Text Notes 675  7050 0    50   Italic 10
Infra-red LED
Text Notes 3050 6425 0    50   Italic 10
Phototransistor current
Text Label 2075 5775 0    50   ~ 0
Vphoto
Text Notes 3050 7600 0    50   ~ 0
To get Vphoto high enough for I(light) = 0.5 mA,\nput R11 = 5.6 kΩ (gives Vphoto = 2.8 V)\n\n(If I(light) is much larger, the phototransistor\ngoes into saturation.)
Text Notes 3050 7150 0    50   Italic 10
Phototransistor current conversion
Text Notes 4425 5050 0    50   Italic 10
Comparator hysteresis
Text Notes 4425 5400 0    50   ~ 0
With comparator outputs 0 and 3.3 V,\nR9 = 7.5 kΩ, R7 = 240 kΩ gives\n\nV(hyst) = 7.5K/(7.5K + 240K) * (3.3 - 0) = 0.1 V
Text Notes 4425 4500 0    50   Italic 10
Comparator threshold
Text Notes 4425 4850 0    50   ~ 0
R8 and R12 set the comparator threshold\nat Vcc/2 = 1.65 V. This is large enough to\navoid problems with ambient light "dark\ncurrent" from the phototransistor.
$Comp
L mini-mapper:LMV331 U4
U 1 1 5F1EDF5B
P 3575 5875
F 0 "U4" H 3675 6240 50  0000 C CNN
F 1 "LMV331" H 3675 6149 50  0000 C CNN
F 2 "mini-mapper:SOT-23-5" H 3675 5975 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lmv331.pdf" H 3675 6075 50  0001 C CNN
F 4 "LMV331IDBVR" H 3575 5875 50  0001 C CNN "MPN"
	1    3575 5875
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:LMV331 U4
U 2 1 5F1EE91F
P 9575 3175
F 0 "U4" H 9700 3225 50  0000 L CNN
F 1 "LMV331" H 9700 3125 50  0000 L CNN
F 2 "mini-mapper:SOT-23-5" H 9675 3275 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lmv331.pdf" H 9675 3375 50  0001 C CNN
F 4 "LMV331IDBVR" H 9575 3175 50  0001 C CNN "MPN"
	2    9575 3175
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR010
U 1 1 5F1F2C41
P 9575 2775
F 0 "#PWR010" H 9575 2625 50  0001 C CNN
F 1 "+3V3" H 9590 2948 50  0000 C CNN
F 2 "" H 9575 2775 50  0001 C CNN
F 3 "" H 9575 2775 50  0001 C CNN
	1    9575 2775
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR015
U 1 1 5F1F32D3
P 9575 3550
F 0 "#PWR015" H 9575 3300 50  0001 C CNN
F 1 "GND" H 9580 3377 50  0000 C CNN
F 2 "" H 9575 3550 50  0001 C CNN
F 3 "" H 9575 3550 50  0001 C CNN
	1    9575 3550
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:C C6
U 1 1 5F1FB2DE
P 8100 3175
F 0 "C6" H 8215 3221 50  0000 L CNN
F 1 "0.1U" H 8215 3130 50  0000 L CNN
F 2 "mini-mapper:C_0805" H 8138 3025 50  0001 C CNN
F 3 "http://datasheet.octopart.com/885012207098-W%C3%BCrth-Elektronik-datasheet-137006532.pdf" H 8100 3175 50  0001 C CNN
F 4 "885012207098" H 8100 3175 50  0001 C CNN "MPN"
	1    8100 3175
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:C C7
U 1 1 5F1FB700
P 9150 3200
F 0 "C7" H 9265 3246 50  0000 L CNN
F 1 "0.1U" H 9265 3155 50  0000 L CNN
F 2 "mini-mapper:C_0805" H 9188 3050 50  0001 C CNN
F 3 "http://datasheet.octopart.com/885012207098-W%C3%BCrth-Elektronik-datasheet-137006532.pdf" H 9150 3200 50  0001 C CNN
F 4 "885012207098" H 9150 3200 50  0001 C CNN "MPN"
	1    9150 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	9150 2875 9150 3050
Wire Wire Line
	9150 3350 9150 3525
Wire Wire Line
	8100 3025 8100 2875
Wire Wire Line
	8100 3325 8100 3475
$Comp
L mini-mapper:TestPoint TP1
U 1 1 5F1806B0
P 1900 5625
F 0 "TP1" H 1958 5743 50  0000 L CNN
F 1 "TestPoint" H 1958 5652 50  0000 L CNN
F 2 "mini-mapper:TestPoint_Pad_2.0x2.0mm" H 2100 5625 50  0001 C CNN
F 3 "n/a" H 2100 5625 50  0001 C CNN
F 4 "n/a" H 1900 5625 50  0001 C CNN "MPN"
	1    1900 5625
	1    0    0    -1  
$EndComp
Wire Wire Line
	1900 5625 1900 5775
Connection ~ 1900 5775
Wire Wire Line
	1900 5775 2750 5775
$Comp
L mini-mapper:Barrel_Jack J1
U 1 1 5F1B8644
P 8575 1675
F 0 "J1" H 8632 2000 50  0000 C CNN
F 1 "PJ-063AH" H 8632 1909 50  0000 C CNN
F 2 "mini-mapper:BarrelJack_CUI_PJ-063AH_Horizontal" H 8625 1635 50  0001 C CNN
F 3 "https://www.mouser.at/datasheet/2/670/pj-063ah-1778660.pdf" H 8625 1635 50  0001 C CNN
F 4 "PJ-063AH" H 8575 1675 50  0001 C CNN "MPN"
	1    8575 1675
	1    0    0    -1  
$EndComp
Wire Wire Line
	8100 2875 8525 2875
Connection ~ 8525 2875
Wire Wire Line
	8100 3475 8525 3475
Connection ~ 8525 3475
Wire Wire Line
	9575 2775 9575 2875
Wire Wire Line
	9575 3475 9575 3525
Wire Wire Line
	9150 2875 9575 2875
Connection ~ 9575 2875
Wire Wire Line
	9150 3525 9575 3525
Connection ~ 9575 3525
Wire Wire Line
	9575 3525 9575 3550
Text Notes 7975 850  0    150  ~ 30
POWER
$Comp
L mini-mapper:TLV70245_SOT23-5 U1
U 1 1 5F2054D4
P 9825 1675
F 0 "U1" H 9825 2017 50  0000 C CNN
F 1 "TLV70245_SOT23-5" H 9825 1926 50  0000 C CNN
F 2 "mini-mapper:SOT-23-5" H 9825 2000 50  0001 C CIN
F 3 "http://www.ti.com/lit/ds/symlink/tlv702.pdf" H 9825 1725 50  0001 C CNN
F 4 "TLV70245DBVR" H 9825 1675 50  0001 C CNN "MPN"
	1    9825 1675
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR05
U 1 1 5F2070D8
P 9825 2050
F 0 "#PWR05" H 9825 1800 50  0001 C CNN
F 1 "GND" H 9830 1877 50  0000 C CNN
F 2 "" H 9825 2050 50  0001 C CNN
F 3 "" H 9825 2050 50  0001 C CNN
	1    9825 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	9825 2050 9825 1975
$Comp
L power:GND #PWR06
U 1 1 5F20FAF2
P 8975 2075
F 0 "#PWR06" H 8975 1825 50  0001 C CNN
F 1 "GND" H 8980 1902 50  0000 C CNN
F 2 "" H 8975 2075 50  0001 C CNN
F 3 "" H 8975 2075 50  0001 C CNN
	1    8975 2075
	1    0    0    -1  
$EndComp
Wire Wire Line
	8875 1775 8975 1775
Wire Wire Line
	8975 1775 8975 2025
Wire Wire Line
	9525 1675 9475 1675
Wire Wire Line
	9475 1675 9475 1575
Connection ~ 9475 1575
Wire Wire Line
	9475 1575 9525 1575
Wire Wire Line
	8875 1575 9075 1575
$Comp
L mini-mapper:C C3
U 1 1 5F223AE5
P 9275 1825
F 0 "C3" H 9390 1871 50  0000 L CNN
F 1 "1U" H 9390 1780 50  0000 L CNN
F 2 "mini-mapper:C_0805" H 9313 1675 50  0001 C CNN
F 3 "http://datasheet.octopart.com/885012107015-W%C3%BCrth-Elektronik-datasheet-82513951.pdf" H 9275 1825 50  0001 C CNN
F 4 "885012107015" H 9275 1825 50  0001 C CNN "MPN"
	1    9275 1825
	1    0    0    -1  
$EndComp
Wire Wire Line
	9275 1675 9275 1575
Connection ~ 9275 1575
Wire Wire Line
	9275 1575 9475 1575
$Comp
L power:GND #PWR07
U 1 1 5F228958
P 9275 2075
F 0 "#PWR07" H 9275 1825 50  0001 C CNN
F 1 "GND" H 9280 1902 50  0000 C CNN
F 2 "" H 9275 2075 50  0001 C CNN
F 3 "" H 9275 2075 50  0001 C CNN
	1    9275 2075
	1    0    0    -1  
$EndComp
Wire Wire Line
	9275 2075 9275 1975
Wire Wire Line
	10125 1575 10300 1575
$Comp
L mini-mapper:C C4
U 1 1 5F233EC6
P 10300 1825
F 0 "C4" H 10415 1871 50  0000 L CNN
F 1 "1U" H 10415 1780 50  0000 L CNN
F 2 "mini-mapper:C_0805" H 10338 1675 50  0001 C CNN
F 3 "http://datasheet.octopart.com/885012107015-W%C3%BCrth-Elektronik-datasheet-82513951.pdf" H 10300 1825 50  0001 C CNN
F 4 "885012107015" H 10300 1825 50  0001 C CNN "MPN"
	1    10300 1825
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR08
U 1 1 5F233ECD
P 10300 2075
F 0 "#PWR08" H 10300 1825 50  0001 C CNN
F 1 "GND" H 10305 1902 50  0000 C CNN
F 2 "" H 10300 2075 50  0001 C CNN
F 3 "" H 10300 2075 50  0001 C CNN
	1    10300 2075
	1    0    0    -1  
$EndComp
Wire Wire Line
	10300 2075 10300 1975
Wire Wire Line
	10300 1675 10300 1575
Connection ~ 10300 1575
$Comp
L mini-mapper:VM #PWR04
U 1 1 5F23CFD7
P 10625 1500
F 0 "#PWR04" H 10425 1350 50  0001 C CNN
F 1 "VM" H 10640 1673 50  0000 C CNN
F 2 "" H 10625 1500 50  0001 C CNN
F 3 "" H 10625 1500 50  0001 C CNN
	1    10625 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	10625 1500 10625 1575
Wire Notes Line
	475  4200 11225 4200
Wire Notes Line
	7825 4200 7825 475 
Wire Wire Line
	8800 2025 8975 2025
Connection ~ 8975 2025
Wire Wire Line
	8975 2025 8975 2075
Text Notes 8250 1550 3    50   ~ 0
5V input
Wire Notes Line
	6975 6550 6975 4200
Text Notes 7075 4500 0    150  ~ 30
NUCLEO CONNECTIONS
Wire Wire Line
	6400 2350 7275 2350
Wire Wire Line
	4100 5875 5175 5875
Text Label 4550 5875 0    50   ~ 0
ENCODER_PULSE
$Comp
L power:+3V3 #PWR017
U 1 1 5F1BF659
P 8400 5125
F 0 "#PWR017" H 8400 4975 50  0001 C CNN
F 1 "+3V3" H 8415 5298 50  0000 C CNN
F 2 "" H 8400 5125 50  0001 C CNN
F 3 "" H 8400 5125 50  0001 C CNN
	1    8400 5125
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR018
U 1 1 5F1BFF71
P 8400 5825
F 0 "#PWR018" H 8400 5575 50  0001 C CNN
F 1 "GND" H 8405 5652 50  0000 C CNN
F 2 "" H 8400 5825 50  0001 C CNN
F 3 "" H 8400 5825 50  0001 C CNN
	1    8400 5825
	1    0    0    -1  
$EndComp
Wire Wire Line
	8400 5125 8400 5150
Wire Wire Line
	8400 5225 8500 5225
Wire Wire Line
	8500 5725 8400 5725
Wire Wire Line
	8400 5725 8400 5825
Wire Wire Line
	8500 5325 7450 5325
Wire Wire Line
	8500 5425 7450 5425
Text Label 7450 5425 0    50   ~ 0
MOTOR_IN1
Text Label 7450 5525 0    50   ~ 0
MOTOR_IN2
Wire Wire Line
	8500 5525 7450 5525
Wire Wire Line
	8500 5625 7450 5625
Text Label 7450 5625 0    50   ~ 0
MOTOR_SENSE
Text Label 7450 5325 0    50   ~ 0
ENCODER_PULSE
Text Notes 8850 5250 0    50   ~ 0
+3V3
Text Notes 8850 5750 0    50   ~ 0
GND
Text Notes 8850 5450 0    50   ~ 0
DIGITAL OUTPUT (PD14, TIM4_CH3)
Text Notes 8850 5550 0    50   ~ 0
DIGITAL OUTPUT (PD15, TIM4_CH4)
Text Notes 8850 5650 0    50   ~ 0
ANALOG INPUT (PA3, ADC123_IN3)
Text Notes 8850 5350 0    50   ~ 0
DIGITAL INPUT (PG3)
Text Notes 8850 5125 0    75   ~ 15
NUCLEO PIN
Wire Wire Line
	10300 1575 10625 1575
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 5F1FD846
P 9075 1500
F 0 "#FLG0101" H 9075 1575 50  0001 C CNN
F 1 "PWR_FLAG" H 8900 1650 50  0000 L CNN
F 2 "" H 9075 1500 50  0001 C CNN
F 3 "~" H 9075 1500 50  0001 C CNN
	1    9075 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	9075 1500 9075 1575
Connection ~ 9075 1575
Wire Wire Line
	9075 1575 9275 1575
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 5F202748
P 8300 5150
F 0 "#FLG0102" H 8300 5225 50  0001 C CNN
F 1 "PWR_FLAG" V 8300 5278 50  0000 L CNN
F 2 "" H 8300 5150 50  0001 C CNN
F 3 "~" H 8300 5150 50  0001 C CNN
	1    8300 5150
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8300 5150 8400 5150
Connection ~ 8400 5150
Wire Wire Line
	8400 5150 8400 5225
Text Notes 10275 5250 0    50   ~ 0
CN8  7
Text Notes 10275 5750 0    50   ~ 0
CN8 11
Text Notes 10275 5450 0    50   ~ 0
CN7 16
Text Notes 10275 5550 0    50   ~ 0
CN7 18
Text Notes 10275 5650 0    50   ~ 0
CN9  1
Text Notes 10275 5350 0    50   ~ 0
CN8 16
$Comp
L mini-mapper:PinHeader_01x06 J3
U 1 1 5F1C4768
P 8700 5425
F 0 "J3" H 8700 5000 50  0000 L CNN
F 1 "PinHeader_01x06" H 8700 4900 50  0000 L CNN
F 2 "mini-mapper:PinHeader_1x06_P2.54mm_Vertical" H 8700 5425 50  0001 C CNN
F 3 "https://www.mouser.at/datasheet/2/276/0022284060_PCB_HEADERS-228129.pdf" H 8700 5425 50  0001 C CNN
F 4 "22-28-4060" H 8700 5425 50  0001 C CNN "MPN"
	1    8700 5425
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 2325 3975 2325
Wire Wire Line
	3650 2700 3975 2700
Text Notes 825  1575 0    50   ~ 0
f(PWM) = 31.25 kHz
Wire Wire Line
	1025 1750 3050 1750
Wire Wire Line
	1025 1850 3050 1850
$Comp
L mini-mapper:MountingHole H1
U 1 1 5F1F1C10
P 10225 2875
F 0 "H1" H 10325 2921 50  0000 L CNN
F 1 "MountingHole" H 10325 2830 50  0000 L CNN
F 2 "mini-mapper:MountingHole_2.7mm_M2.5" H 10225 2875 50  0001 C CNN
F 3 "n/a" H 10225 2875 50  0001 C CNN
F 4 "n/a" H 10225 2875 50  0001 C CNN "MPN"
	1    10225 2875
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:MountingHole H2
U 1 1 5F1F20F2
P 10225 3075
F 0 "H2" H 10325 3121 50  0000 L CNN
F 1 "MountingHole" H 10325 3030 50  0000 L CNN
F 2 "mini-mapper:MountingHole_2.7mm_M2.5" H 10225 3075 50  0001 C CNN
F 3 "n/a" H 10225 3075 50  0001 C CNN
F 4 "n/a" H 10225 3075 50  0001 C CNN "MPN"
	1    10225 3075
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:MountingHole H3
U 1 1 5F1F24C2
P 10225 3275
F 0 "H3" H 10325 3321 50  0000 L CNN
F 1 "MountingHole" H 10325 3230 50  0000 L CNN
F 2 "mini-mapper:MountingHole_2.7mm_M2.5" H 10225 3275 50  0001 C CNN
F 3 "n/a" H 10225 3275 50  0001 C CNN
F 4 "n/a" H 10225 3275 50  0001 C CNN "MPN"
	1    10225 3275
	1    0    0    -1  
$EndComp
$Comp
L mini-mapper:MountingHole H4
U 1 1 5F1F297E
P 10225 3475
F 0 "H4" H 10325 3521 50  0000 L CNN
F 1 "MountingHole" H 10325 3430 50  0000 L CNN
F 2 "mini-mapper:MountingHole_2.7mm_M2.5" H 10225 3475 50  0001 C CNN
F 3 "n/a" H 10225 3475 50  0001 C CNN
F 4 "n/a" H 10225 3475 50  0001 C CNN "MPN"
	1    10225 3475
	1    0    0    -1  
$EndComp
Text Label 3975 1850 0    50   ~ 0
MOTOR1
Wire Wire Line
	3850 1750 4400 1750
Wire Wire Line
	3850 1850 4400 1850
Text Label 3975 1750 0    50   ~ 0
MOTOR2
Text Label 8900 1575 0    50   ~ 0
VIN
$EndSCHEMATC
