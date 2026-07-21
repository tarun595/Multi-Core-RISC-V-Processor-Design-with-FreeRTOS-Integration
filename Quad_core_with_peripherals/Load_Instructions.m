% --- MATLAB Script: setup_simulation.m ---

% --- 1. Read the single binary file ---
fileID = fopen('firmware.hex', 'r');
if fileID == -1
    error('Could not open firmware.bin. Make sure the file is in the correct path.');
end
% Read the contents into a vector of 32-bit unsigned integers
instruction_memory_data = fread(fileID, inf, 'uint32=>uint32');
fclose(fileID);

fprintf('Successfully read %d instructions from firmware.bin.\n', numel(instruction_memory_data));

% --- 2. Define model and block names ---
MODEL_NAME = 'processor_with_peripherals'; % <<<--- REPLACE THIS
% A list of all instruction memory block names
all_instruction_memories = {
    'Instruction_Memory_0', 
    'Instruction_Memory_1', 
    'Instruction_Memory_2', 
    'Instruction_Memory_3'
};

% --- 3. Loop through and program each core's memory ---
disp('Loading program into all four cores...');
for i = 1:length(all_instruction_memories)
    block_name = all_instruction_memories{i};
    
    % Find the full path to the block
    full_block_path_cell = find_system(MODEL_NAME, 'Name', block_name);
    
    if isempty(full_block_path_cell)
        warning('Could not find block named "%s" in model "%s". Skipping.', block_name, MODEL_NAME);
    else
        full_block_path = full_block_path_cell{1};
        
        % Use set_param to load the instruction data into the block's 'Table' parameter
        set_param(full_block_path, 'Table', mat2str(instruction_memory_data));
        fprintf('Successfully loaded program into %s.\n', block_name);
    end
end

disp('--- FreeRTOS binary loaded into all cores. Ready for simulation. ---');