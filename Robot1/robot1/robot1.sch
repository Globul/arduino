EESchema Schematic File Version 4
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Driver_Motor:L298HN U?
U 1 1 5BC39EAB
P 7800 1650
F 0 "U?" H 7800 2528 50  0000 C CNN
F 1 "L298HN" H 7800 2437 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-15_P2.54x2.54mm_StaggerOdd_Lead4.58mm_Vertical" H 7850 1000 50  0001 L CNN
F 3 "http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/CD00000240.pdf" H 7950 1900 50  0001 C CNN
	1    7800 1650
	1    0    0    -1  
$EndComp
$Comp
L Interface_Optical:TSOP331xx U?
U 1 1 5BC3A1B4
P 3550 1400
F 0 "U?" H 3537 1825 50  0000 C CNN
F 1 "TSOP331xx" H 3537 1734 50  0000 C CNN
F 2 "OptoDevice:Vishay_MINIMOLD-3Pin" H 3500 1025 50  0001 C CNN
F 3 "http://www.vishay.com/docs/82742/tsop331.pdf" H 4200 1700 50  0001 C CNN
	1    3550 1400
	1    0    0    -1  
$EndComp
$Comp
L Device:Battery BT?
U 1 1 5BC3A24B
P 1700 1700
F 0 "BT?" H 1808 1746 50  0000 L CNN
F 1 "Battery" H 1808 1655 50  0000 L CNN
F 2 "" V 1700 1760 50  0001 C CNN
F 3 "~" V 1700 1760 50  0001 C CNN
	1    1700 1700
	1    0    0    -1  
$EndComp
$Comp
L Analog_Switch:ADG417BN U?
U 1 1 5BC3A33B
P 1550 2550
F 0 "U?" H 1550 2817 50  0000 C CNN
F 1 "ADG417BN" H 1550 2726 50  0000 C CNN
F 2 "Package_DIP:DIP-8_W7.62mm" H 1550 2450 50  0001 C CNN
F 3 "http://www.analog.com/media/en/technical-documentation/data-sheets/ADG417.pdf" H 1550 2550 50  0001 C CNN
	1    1550 2550
	1    0    0    -1  
$EndComp
$Comp
L Motor:Motor_DC M?
U 1 1 5BC3A3C3
P 9850 1500
F 0 "M?" H 10008 1496 50  0000 L CNN
F 1 "Motor_DC" H 10008 1405 50  0000 L CNN
F 2 "" H 9850 1410 50  0001 C CNN
F 3 "~" H 9850 1410 50  0001 C CNN
	1    9850 1500
	1    0    0    -1  
$EndComp
$Comp
L Motor:Motor_DC M?
U 1 1 5BC3A41D
P 10600 1600
F 0 "M?" H 10758 1596 50  0000 L CNN
F 1 "Motor_DC" H 10758 1505 50  0000 L CNN
F 2 "" H 10600 1510 50  0001 C CNN
F 3 "~" H 10600 1510 50  0001 C CNN
	1    10600 1600
	1    0    0    -1  
$EndComp
$Comp
L Motor:Motor_DC M?
U 1 1 5BC3A497
P 10600 2350
F 0 "M?" H 10758 2346 50  0000 L CNN
F 1 "Motor_DC" H 10758 2255 50  0000 L CNN
F 2 "" H 10600 2260 50  0001 C CNN
F 3 "~" H 10600 2260 50  0001 C CNN
	1    10600 2350
	1    0    0    -1  
$EndComp
$Comp
L Motor:Motor_DC M?
U 1 1 5BC3A4ED
P 9850 2300
F 0 "M?" H 10008 2296 50  0000 L CNN
F 1 "Motor_DC" H 10008 2205 50  0000 L CNN
F 2 "" H 9850 2210 50  0001 C CNN
F 3 "~" H 9850 2210 50  0001 C CNN
	1    9850 2300
	1    0    0    -1  
$EndComp
$EndSCHEMATC
