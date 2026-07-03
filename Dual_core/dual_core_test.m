% =========================================================================
% SETUP_MULTICORE_BUSES.M
% This script defines all Simulink.Bus objects for the dual-core processor,
% using the specific signal names from your model.
%
% Instructions: Run this script once before starting the simulation.
% =========================================================================

disp('Creating all pipeline bus objects for the dual-core system...');

%% --- 1. IF/ID Bus Definition ---
% Carries signals from Fetch to Decode.
clear elems; 

elems(1) = Simulink.BusElement;
elems(1).Name = 'Instruction';
elems(1).DataType = 'uint32';

elems(2) = Simulink.BusElement;
elems(2).Name = 'PC';
elems(2).DataType = 'uint32'; % Using your preferred uint32 type

IFID_Bus = Simulink.Bus;
IFID_Bus.Elements = elems;

%% --- 2. ID/EX Bus Definition (Flat Structure) ---
% Carries all decoded signals to the Execute stage.
clear elems;

elems(2) = Simulink.BusElement; elems(2).Name = 'RegWrite'; elems(2).DataType = 'boolean';
elems(4) = Simulink.BusElement; elems(4).Name = 'ALUSrc';   elems(4).DataType = 'boolean';
elems(3) = Simulink.BusElement; elems(3).Name = 'MemRead';  elems(3).DataType = 'boolean';
elems(7) = Simulink.BusElement; elems(7).Name = 'MemWrite'; elems(7).DataType = 'boolean';
elems(5) = Simulink.BusElement; elems(5).Name = 'Branch';   elems(5).DataType = 'uint32';
elems(13) = Simulink.BusElement; elems(13).Name = 'Jump';     elems(13).DataType = 'boolean';
elems(14) = Simulink.BusElement; elems(14).Name = 'PCSrc';    elems(14).DataType = 'uint32';
elems(6) = Simulink.BusElement; elems(6).Name = 'ALUOp';    elems(6).DataType = 'uint32'; elems(6).Dimensions = [1 2];
elems(12) = Simulink.BusElement; elems(12).Name = 'PC';           elems(12).DataType = 'uint32';
elems(8) = Simulink.BusElement; elems(8).Name = 'read_data_1';  elems(8).DataType = 'int32';
elems(9) = Simulink.BusElement; elems(9).Name = 'read_data_2';  elems(9).DataType = 'int32';
elems(1) = Simulink.BusElement; elems(1).Name = 'immediate_out';    elems(1).DataType = 'int32';
elems(11) = Simulink.BusElement; elems(11).Name = 'rd';           elems(11).DataType = 'uint8';
elems(10) = Simulink.BusElement; elems(10).Name = 'Instruction';  elems(10).DataType = 'uint32';
elems(15) = Simulink.BusElement; elems(15).Name = 'AtomicOp';  elems(15).DataType = 'boolean';

IDEX_Bus = Simulink.Bus;
IDEX_Bus.Elements = elems;

%% --- 3. EX/MEM Bus Definition (Flat Structure) ---
% Carries signals to the Memory Access stage.
clear elems;


elems(6) = Simulink.BusElement; elems(6).Name = 'RegWrite'; elems(6).DataType = 'boolean';
elems(5) = Simulink.BusElement; elems(5).Name = 'MemRead';  elems(5).DataType = 'boolean';
elems(4) = Simulink.BusElement; elems(4).Name = 'MemWrite'; elems(4).DataType = 'boolean';
elems(1) = Simulink.BusElement; elems(1).Name = 'ALU_Result';    elems(1).DataType = 'int32';
elems(2) = Simulink.BusElement; elems(2).Name = 'read_data_2';   elems(2).DataType = 'int32';
elems(3) = Simulink.BusElement; elems(3).Name = 'rd';            elems(3).DataType = 'uint8';
elems(7) = Simulink.BusElement; elems(7).Name = 'AtomicOp';  elems(7).DataType = 'boolean';

EXMEM_Bus = Simulink.Bus;
EXMEM_Bus.Elements = elems;

%% --- 4. MEM/WB Bus Definition (Flat Structure) ---
% Carries the final results to the Write-Back stage.
clear elems;

elems(2) = Simulink.BusElement; elems(2).Name = 'RegWrite'; elems(2).DataType = 'boolean';
elems(1) = Simulink.BusElement; elems(1).Name = 'ALU_Result';         elems(1).DataType = 'int32';
elems(4) = Simulink.BusElement; elems(4).Name = 'rd';                 elems(4).DataType = 'uint8';
elems(3) = Simulink.BusElement; elems(3).Name= 'MemRead';             elems(3).DataType = 'boolean';

MEMWB_Bus = Simulink.Bus;
MEMWB_Bus.Elements = elems;

disp('All FLAT pipeline bus objects created successfully.');
