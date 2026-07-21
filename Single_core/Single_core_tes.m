% --- Script to load machine code into the model ---

% The machine code from your assembler

hex_code = {
    '00500093',  % addi x1, x0, 5
    '00a00113',  % addi x2, x0, 10
    '002081b3',  % add  x3, x1, x2
    '00302223',  % sw   x3, 4(x0)
    '00402203',  % lw   x4, 4(x0)
    '00418463',  % beq  x3, x4, L1
    '06300293',  % addi x5, x0, 99
    '00100313',  % L1: addi x6, x0, 1
    '0000006f'   % DONE: jal x0, DONE 
};
hex_code = {
    '00C00093',  % addi x1, x0, 12
    '00102023',  % sw   x1, 0(x0)
    '00002103',  % lw   x2, 0(x0)
    'FFF10113',  % addi x2, x2, -1
    '00202023',  % sw   x2, 0(x0)
    '00112193',  % slti x3, x2, 1
    'FE0188E3',  % beq  x3, x0, LOOP (branches -16 bytes backward)
    '00008067'   % jalr x0, x1, 0
};


% Convert hex strings to a decimal vector
machine_code = hex2dec(hex_code);

% Get the path to your Instruction Memory block
% IMPORTANT: Replace 'Single_Core/...' with the actual path in your model
block_path = 'Single_Core/Instruction Fetch (IF) Stage/Direct Lookup Table (n-D)';

% Set the 'TableData' parameter of the lookup table
set_param(block_path, 'Table', mat2str(machine_code));

disp('Machine code has been loaded into the Instruction Memory.');