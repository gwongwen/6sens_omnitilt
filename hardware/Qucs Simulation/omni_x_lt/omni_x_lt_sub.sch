<Qucs Schematic 0.0.19>
<Properties>
  <View=243,70,1270,559,1.52775,0,0>
  <Grid=10,10,1>
  <DataSet=omni_x_lt_sub.dat>
  <DataDisplay=omni_x_lt_sub.dpl>
  <OpenDisplay=1>
  <Script=omni_x_lt_sub.m>
  <RunScript=0>
  <showFrame=0>
  <FrameText0=Title>
  <FrameText1=Drawn By:>
  <FrameText2=Date:>
  <FrameText3=Revision:>
</Properties>
<Symbol>
  <.PortSym 40 20 2 0>
  <.PortSym 40 60 3 0>
  <.PortSym 40 100 1 0>
</Symbol>
<Components>
  <C Cc1 1 930 280 -26 17 0 0 "100 mF" 1 "" 0 "neutral" 0>
  <C Cc2 1 930 380 -26 17 0 0 "100 mF" 1 "" 0 "neutral" 0>
  <R Rc1 1 990 200 -26 15 0 0 "900 Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <R Rc2 1 990 480 -26 15 0 0 "900 Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <L Lc1 1 890 200 -26 10 0 0 "70 mH" 1 "" 0>
  <L Lc2 1 890 480 -26 10 0 0 "70 mH" 1 "" 0>
  <GND * 1 330 400 0 0 0 0>
  <C Cm1 1 420 260 -26 17 0 0 "0.012 F" 1 "" 0 "neutral" 0>
  <L Lm1 1 620 320 -26 10 0 0 "0.00938 H" 1 "" 0>
  <R Rm1 1 490 330 15 -26 0 1 "0.650 Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <Tr Tm1 1 720 330 -29 38 0 0 "0.045" 1>
  <GND * 1 790 400 0 0 0 0>
  <Port Out1 1 1110 220 -23 12 0 0 "2" 1 "analog" 0>
  <Port Out2 1 1110 440 -23 12 0 0 "3" 1 "analog" 0>
  <Port In1 1 310 200 -23 12 0 0 "1" 1 "analog" 0>
  <Vac Vac1 0 330 320 18 -26 0 1 "0.5 V" 1 "1 Hz" 1 "0" 0 "0" 0>
  <.AC AC1 0 360 400 0 33 0 0 "lin" 1 "1 Hz" 1 "1000 Hz" 1 "1001" 1 "no" 0>
  <Eqn Eqn1 0 510 410 -23 13 0 0 "gain_dB=dB((Vout1.v-Vout2.v)/Vin.v)" 1 "sensitivity=abs((Vout1.v-Vout2.v)/Vin.v)" 1 "phase=rad2deg(unwrap(angle((Vout1.v-Vout2.v)/Vin.v)))" 1 "yes" 0>
  <R Rsh1 0 1150 340 15 -26 0 1 "9.76 Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
</Components>
<Wires>
  <920 200 960 200 "" 0 0 0 "">
  <830 280 900 280 "" 0 0 0 "">
  <830 200 830 280 "" 0 0 0 "">
  <830 200 860 200 "" 0 0 0 "">
  <960 280 1050 280 "" 0 0 0 "">
  <1050 200 1050 280 "" 0 0 0 "">
  <1020 200 1050 200 "" 0 0 0 "">
  <1020 480 1050 480 "" 0 0 0 "">
  <920 480 960 480 "" 0 0 0 "">
  <830 480 860 480 "" 0 0 0 "">
  <830 380 830 480 "" 0 0 0 "">
  <1050 380 1050 480 "" 0 0 0 "">
  <960 380 1050 380 "" 0 0 0 "">
  <830 380 900 380 "" 0 0 0 "">
  <490 260 490 300 "" 0 0 0 "">
  <490 260 590 260 "" 0 0 0 "">
  <490 360 490 380 "" 0 0 0 "">
  <590 260 590 320 "" 0 0 0 "">
  <450 260 490 260 "" 0 0 0 "">
  <690 360 690 380 "" 0 0 0 "">
  <830 360 830 380 "" 0 0 0 "">
  <830 280 830 300 "" 0 0 0 "">
  <750 300 830 300 "" 0 0 0 "">
  <590 260 690 260 "" 0 0 0 "">
  <690 260 690 300 "" 0 0 0 "">
  <330 380 490 380 "" 0 0 0 "">
  <330 350 330 380 "" 0 0 0 "">
  <330 260 390 260 "Vin" 370 230 26 "">
  <330 260 330 290 "" 0 0 0 "">
  <330 380 330 400 "" 0 0 0 "">
  <750 360 790 360 "" 0 0 0 "">
  <650 320 670 320 "" 0 0 0 "">
  <490 380 670 380 "" 0 0 0 "">
  <670 380 690 380 "" 0 0 0 "">
  <670 320 670 380 "" 0 0 0 "">
  <1050 200 1150 200 "Vout1" 1130 180 74 "">
  <1150 200 1150 220 "" 0 0 0 "">
  <1050 480 1150 480 "Vout2" 1130 490 80 "">
  <1110 220 1150 220 "" 0 0 0 "">
  <1150 440 1150 480 "" 0 0 0 "">
  <1110 440 1150 440 "" 0 0 0 "">
  <310 200 330 200 "" 0 0 0 "">
  <330 200 330 260 "" 0 0 0 "">
  <790 360 830 360 "" 0 0 0 "">
  <790 360 790 400 "" 0 0 0 "">
  <1150 220 1150 310 "" 0 0 0 "">
  <1150 370 1150 440 "" 0 0 0 "">
</Wires>
<Diagrams>
</Diagrams>
<Paintings>
  <Text 400 110 36 #0000ff 0 "Geophone OMNI-X-LT Equivalent Circuit">
</Paintings>