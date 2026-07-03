% =========================================================================
% SETUP_MULTICORE_BUSES.M
% This script defines all Simulink.Bus objects for the dual-core processor,
% using the specific signal names from your model.
%
% Instructions: Run this script once before starting the simulation.
% =========================================================================

disp('Creating all pipeline bus objects for the quad-core system...');

%% --- 1. CH/0 Bus Definition ---
% Carries signals from Fetch to Decode.
clear elems; 

elems(1) = Simulink.BusElement; elems(1).Name = 'Mem_RData_0'; elems(1).DataType = 'int32';
elems(2) = Simulink.BusElement; elems(2).Name = 'IRQ_0';         elems(2).DataType = 'boolean';
elems(3) = Simulink.BusElement; elems(3).Name = 'Mem_RData_1';                 elems(3).DataType = 'int32';
elems(4) = Simulink.BusElement; elems(4).Name= 'IRQ_1';             elems(4).DataType = 'boolean';

elems(5) = Simulink.BusElement; elems(5).Name = 'Mem_RData_2'; elems(5).DataType = 'int32';
elems(6) = Simulink.BusElement; elems(6).Name = 'IRQ_2';         elems(6).DataType = 'boolean';
elems(7) = Simulink.BusElement; elems(7).Name = 'Mem_RData_3';                 elems(7).DataType = 'int32';
elems(8) = Simulink.BusElement; elems(8).Name= 'IRQ_3';             elems(8).DataType = 'boolean';

CH0_Bus = Simulink.Bus;
CH0_Bus.Elements = elems;

%% --- 2. 01 Bus Definition (Flat Structure) ---
% Carries all decoded signals to the Execute stage.
clear elems;


elems(1) = Simulink.BusElement; elems(1).Name = 'MemRead_Out_0'; elems(1).DataType = 'boolean';
elems(2) = Simulink.BusElement; elems(2).Name = 'Mem_WriteData_Out_0';         elems(2).DataType = 'int32';
elems(3) = Simulink.BusElement; elems(3).Name = 'Mem_Address_Out_0';                 elems(3).DataType = 'int32';
elems(4) = Simulink.BusElement; elems(4).Name= 'MemWrite_Out_0';             elems(4).DataType = 'boolean';
elems(5) = Simulink.BusElement; elems(5).Name = 'Mem_RData_1'; elems(5).DataType = 'int32';
elems(6) = Simulink.BusElement; elems(6).Name = 'IRQ_1';         elems(6).DataType = 'boolean';
elems(7) = Simulink.BusElement; elems(7).Name = 'Mem_RData_2';                 elems(7).DataType = 'int32';
elems(8) = Simulink.BusElement; elems(8).Name= 'IRQ_2';             elems(8).DataType = 'boolean';
elems(9) = Simulink.BusElement; elems(9).Name = 'Mem_RData_3';                 elems(9).DataType = 'int32';
elems(10) = Simulink.BusElement; elems(10).Name= 'IRQ_3';             elems(10).DataType = 'boolean';

C01_Bus = Simulink.Bus;
C01_Bus.Elements = elems;

%% --- 3. 1/2 Bus Definition (Flat Structure) ---
% Carries signals to the Memory Access stage.
clear elems;


elems(1) = Simulink.BusElement; elems(1).Name = 'MemRead_Out_1'; elems(1).DataType = 'boolean';
elems(2) = Simulink.BusElement; elems(2).Name = 'Mem_WriteData_Out_1';         elems(2).DataType = 'int32';
elems(3) = Simulink.BusElement; elems(3).Name = 'Mem_Address_Out_1';                 elems(3).DataType = 'int32';
elems(4) = Simulink.BusElement; elems(4).Name= 'MemWrite_Out_1';             elems(4).DataType = 'boolean';
elems(5) = Simulink.BusElement; elems(5).Name = 'MemWrite_Out_0'; elems(5).DataType = 'boolean';
elems(6) = Simulink.BusElement; elems(6).Name = 'Mem_Address_Out_0';         elems(6).DataType = 'int32';
elems(7) = Simulink.BusElement; elems(7).Name = 'Mem_WriteData_Out_0';                 elems(7).DataType = 'int32';
elems(8) = Simulink.BusElement; elems(8).Name= 'MemRead_Out_0';             elems(8).DataType = 'boolean';
elems(9) = Simulink.BusElement; elems(9).Name = 'Mem_RData_2';                 elems(9).DataType = 'int32';
elems(10) = Simulink.BusElement; elems(10).Name= 'IRQ_2';             elems(10).DataType = 'boolean';
elems(11) = Simulink.BusElement; elems(11).Name = 'Mem_RData_3';                 elems(11).DataType = 'int32';
elems(12) = Simulink.BusElement; elems(12).Name= 'IRQ_3';             elems(12).DataType = 'boolean';

C12_Bus = Simulink.Bus;
C12_Bus.Elements = elems;

%% --- 3. 2/3 Bus Definition (Flat Structure) ---
% Carries signals to the Memory Access stage.
clear elems;


elems(1) = Simulink.BusElement; elems(1).Name = 'MemRead_Out_2'; elems(1).DataType = 'boolean';
elems(2) = Simulink.BusElement; elems(2).Name = 'Mem_WriteData_Out_2';         elems(2).DataType = 'int32';
elems(3) = Simulink.BusElement; elems(3).Name = 'Mem_Address_Out_2';                 elems(3).DataType = 'int32';
elems(4) = Simulink.BusElement; elems(4).Name= 'MemWrite_Out_2';             elems(4).DataType = 'boolean';
elems(5) = Simulink.BusElement; elems(5).Name = 'MemRead_Out_1'; elems(5).DataType = 'boolean';
elems(6) = Simulink.BusElement; elems(6).Name = 'Mem_WriteData_Out_1';         elems(6).DataType = 'int32';
elems(7) = Simulink.BusElement; elems(7).Name = 'Mem_Address_Out_1';                 elems(7).DataType = 'int32';
elems(8) = Simulink.BusElement; elems(8).Name= 'MemWrite_Out_1';             elems(8).DataType = 'boolean';
elems(9) = Simulink.BusElement; elems(9).Name = 'MemWrite_Out_0';                 elems(9).DataType = 'boolean';
elems(10) = Simulink.BusElement; elems(10).Name= 'Mem_Address_Out_0';             elems(10).DataType = 'int32';
elems(11) = Simulink.BusElement; elems(11).Name = 'Mem_WriteData_Out_0';                 elems(11).DataType = 'int32';
elems(12) = Simulink.BusElement; elems(12).Name= 'MemRead_Out_0';             elems(12).DataType = 'boolean';
elems(13) = Simulink.BusElement; elems(13).Name = 'Mem_RData_3';                 elems(13).DataType = 'int32';
elems(14) = Simulink.BusElement; elems(14).Name= 'IRQ_3';             elems(14).DataType = 'boolean';

C23_Bus = Simulink.Bus;
C23_Bus.Elements = elems;

%% --- 3. 3/CH Bus Definition (Flat Structure) ---
% Carries signals to the Memory Access stage.
clear elems;


elems(1) = Simulink.BusElement; elems(1).Name = 'MemRead_Out_3'; elems(1).DataType = 'boolean';
elems(2) = Simulink.BusElement; elems(2).Name = 'Mem_WriteData_Out_3';         elems(2).DataType = 'int32';
elems(3) = Simulink.BusElement; elems(3).Name = 'Mem_Address_Out_3';                 elems(3).DataType = 'int32';
elems(4) = Simulink.BusElement; elems(4).Name= 'MemWrite_Out_3';             elems(4).DataType = 'boolean';
elems(5) = Simulink.BusElement; elems(5).Name = 'MemRead_Out_1'; elems(5).DataType = 'boolean';
elems(6) = Simulink.BusElement; elems(6).Name = 'MemWrite_Out_2';         elems(6).DataType = 'boolean';
elems(7) = Simulink.BusElement; elems(7).Name = 'Mem_Address_Out_2';                 elems(7).DataType = 'int32';
elems(8) = Simulink.BusElement; elems(8).Name= 'Mem_WriteData_Out_2';             elems(8).DataType = 'int32';
elems(9) = Simulink.BusElement; elems(9).Name = 'MemRead_Out_2';                 elems(9).DataType = 'boolean';
elems(10) = Simulink.BusElement; elems(10).Name= 'Mem_WriteData_Out_1';             elems(10).DataType = 'int32';
elems(11) = Simulink.BusElement; elems(11).Name = 'Mem_Address_Out_1';                 elems(11).DataType = 'int32';
elems(12) = Simulink.BusElement; elems(12).Name= 'MemWrite_Out_1';             elems(12).DataType = 'boolean';
elems(13) = Simulink.BusElement; elems(13).Name = 'MemWrite_Out_0';                 elems(13).DataType = 'boolean';
elems(14) = Simulink.BusElement; elems(14).Name= 'Mem_Address_Out_0';             elems(14).DataType = 'int32';
elems(15) = Simulink.BusElement; elems(15).Name = 'Mem_WriteData_Out_0';                 elems(15).DataType = 'int32';
elems(16) = Simulink.BusElement; elems(16).Name= 'MemRead_Out_0';             elems(16).DataType = 'boolean';

C3CH_Bus = Simulink.Bus;
C3CH_Bus.Elements = elems;

disp('All FLAT pipeline bus objects created successfully.');
