%% MATLAB Script for Post-Processing Testbench Data
% This script imports the data obtained from the automated tests of the P5
% and calculates the average values for each measurement.
% The experiments were conducted by varying the settings every 6 minutes.
% It takes approximately 5 minutes for the P5 to stabilize in the new settings,
% and the last minute of data is the one of interest for analysis.

% Kamilla Peixoto 13/07/2023

clear
clc
close all
%% Define constants

% File name
file_name = 'SystemVerification_Tests.xlsx';

% Range of data to be computed
time_start = 30000;
time_end = 36000;

set_start  = 1;
set_end    = 20;


averages = [];
%% Import Data

rawData = readtable(file_name);

%% Compute the average of all variables

for i = set_start:set_end
    databySet = rawData(rawData.setting_counter == i, :);
    sigDatabySet = databySet((databySet.autotime_counter > time_start)&(databySet.autotime_counter <= time_end), :);
    %size(sigDatabySet);
    averages(i,:) = [i*0.25, mean(sigDatabySet.gfRMSCurrent), mean(sigDatabySet.gfO2concentration), -mean(sigDatabySet.gfFlowSMF), mean(sigDatabySet.gfO2temperature)];
end

% Print average values
averages
%% Save the average in a file

headers = {'Setting', 'RMS Current', 'O2', 'Flow', 'Temperature'};  % Headers for the columns

file_name_avg = 'averages.csv';  % Name of the CSV file

dataTable = array2table(averages, 'VariableNames', headers);  % Create a table with the data and headers
writetable(dataTable, file_name_avg);  % Save the table to the CSV file

%% Compare data with IMT

% Import IMT Data
file_IMT = 'IMT References.xlsx';
data_IMT = readtable(file_IMT);

%% Oxygen Data

% Errors for O2 of the testbench
ac_O2_tb   = 1.5*(95.6-20.5)/100;                     % From datasheet
error_O2_tb = ac_O2_tb*ones(1,length(averages));  % Error constant

% Errors for O2 of the IMT
ac_O2_IMT    = 1;
error_O2_IMT = ac_O2_IMT*ones(1,height(data_IMT));  % Error constant 

% Figure 1: Error of O2 Concentration
figure();
hold on;

% Plot Dataset of TestBench with error bars
errorbar(averages(:, 1),averages(:, 3), error_O2_tb, 'o', 'LineWidth', 1.5, 'Color','green');

% Plot Dataset of IMT with error bars
errorbar(data_IMT.setting, data_IMT.o2, error_O2_IMT, 'o', 'LineWidth', 1.5, 'Color','blue');

% Split graphs
plot ([1.62 1.62], [85 100], '-.', 'color', 'black','LineWidth', 1.5)
plot ([4.37 4.37], [85 100], '-.', 'color', 'black','LineWidth', 1.5)
ylim([85 100]);
yticks([87:3:100]);

% Add text labels
text(0.8, 99, 'I', 'FontSize', 16, 'FontWeight', 'bold')
text(2.95, 99, 'II', 'FontSize', 16, 'FontWeight', 'bold')
text(4.75, 99, 'III', 'FontSize', 16, 'FontWeight', 'bold')

% Add labels and title
xlabel('Settings', 'FontSize', 14);
ylabel('O2 Concentration (%)', 'FontSize', 14);
%title('System Verification Tests', 'FontSize', 16);
xlim([0 5.25])
%xticks(0.25:0.25:5)

% Set font size for axis tick labels
set(gca, 'FontSize', 12);

% Add legend
legend('Testbench', 'IMT', 'Location', 'southeast', 'FontSize', 14);

% Adjust the figure properties
grid on;
hold off;
%%

% Flow Data

% Errors for flow of the testbench
ac_flow_tb = 0.0301;                     % From datasheet
error_flow_tb = 0.1 + ac_flow_tb*averages(:, 4);  %  

% Errors for flow of the IMT
ac_flow_IMT = 0.1;
error_flow_IMT = ac_flow_IMT*ones(1,height(data_IMT));  % Error constant 

% Create a figure
figure();
hold on;

% Plot Dataset of TestBench with error bars
errorbar(averages(:, 1),averages(:, 4), error_flow_tb, 'o', 'LineWidth', 1.5, 'Color','green');

% Plot Dataset of IMT with error bars
errorbar(data_IMT.setting, data_IMT.flow, error_flow_IMT, 'o', 'LineWidth', 1.5, 'Color','blue');


plot ([1.62 1.62], [-0.2 1.3], '-.', 'color', 'black', 'LineWidth', 1.5);
plot ([4.37 4.37], [-0.2 1.3], '-.', 'color', 'black', 'LineWidth', 1.5);
ylim([-0.2 1.3]);

% Add text labels
text(0.8, 1.2, 'I', 'FontSize', 12, 'FontWeight', 'bold')
text(3, 1.2, 'II', 'FontSize', 12, 'FontWeight', 'bold')
text(4.8, 1.2, 'III', 'FontSize', 12, 'FontWeight', 'bold')

% Add labels and title
xlabel('Settings');
ylabel('Flow (L/min)');
%title('System Verification Tests');
xlim([0 5.25])
% Add legend
legend('Testbench', 'IMT', 'Location','southeast');

% Adjust the figure properties
grid on;
hold off;

% Temperature Data

% Errors for temperature of the IMT
ac_temp_IMT = 0.5;
error_temp_IMT = ac_temp_IMT*ones(1,height(data_IMT));  % Error constant 

% Create a figure
figure();
hold on;

% Plot Dataset of TestBench with error bars
plot(averages(:, 1),averages(:, 5), 'o', 'LineWidth', 1.5, 'Color','green');

% Plot Dataset of IMT with error bars
errorbar(data_IMT.setting, data_IMT.temp, error_temp_IMT, 'o', 'LineWidth', 1.5, 'Color','blue');

% Add labels and title
xlabel('Settings');
ylabel('Temperature (°C)');
%title('System Verification Tests');
xlim([0 5.25])
ylim([22 34])

plot ([1.62 1.62], [22 34], '-.', 'color', 'black')
plot ([4.37 4.37], [22 34], '-.', 'color', 'black')

% Add legend
legend('Testbench', 'IMT', 'Location','southeast');

% Add text labels
text(0.8, 33.5, 'I', 'FontSize', 12, 'FontWeight', 'bold')
text(3, 33.5, 'II', 'FontSize', 12, 'FontWeight', 'bold')
text(4.8, 33.5, 'III', 'FontSize', 12, 'FontWeight', 'bold')


% Adjust the figure properties
grid on;
hold off;

%% Check Extra Measurements with low humidity

% Import Testbench Data
file_name_ex = 'first6settings.xlsx';
rawData_ex = readtable(file_name_ex);


% Import IMT Data
data_IMT_ex = readtable(file_IMT, 'Sheet',2);


% Compute the average of all variables
for i = set_start:6
    databySet_ex = rawData_ex(rawData_ex.setting_counter == i, :);
    sigDatabySet_ex = databySet_ex((databySet_ex.autotime_counter > time_start)&(databySet_ex.autotime_counter <= time_end), :);
    size(sigDatabySet_ex)
    averages_ex(i,:) = [i*0.25, mean(sigDatabySet_ex.gfRMSCurrent), mean(sigDatabySet_ex.gfO2concentration), -mean(sigDatabySet_ex.gfFlowSMF), mean(sigDatabySet_ex.gfO2temperature)];
end

error_O2_tb_ex = ac_O2_tb*ones(1,length(averages_ex));  % Error constant
error_O2_IMT_ex = ac_O2_IMT*ones(1,height(data_IMT_ex));  % Error constant 

% Errors for flow 
error_flow_tb_ex = 0.1 + ac_flow_tb*data_IMT_ex.flow;  %  
error_flow_IMT_ex = ac_flow_IMT*ones(1,height(data_IMT_ex));  % Error constant 

% Errors for temperature of the IMT
error_temp_IMT_ex = ac_temp_IMT*ones(1,height(data_IMT_ex));  % Error constant 


% Create a figure
figure();
hold on;


% Plot Dataset of Procheck with error bars
error_procheck = 2*ones(1,6);
errorbar(averages_ex(:, 1),96*ones(1,6), error_procheck, 'o', 'LineWidth', 1.5, 'Color','red');

% Plot Dataset of TestBench with error bars
errorbar(averages_ex(:, 1),averages_ex(:, 3), error_O2_tb_ex, 'o', 'LineWidth', 1.5, 'Color','green');

% Plot Dataset of IMT with error bars
errorbar(data_IMT_ex.setting(1:6), data_IMT_ex.o2(1:6), error_O2_IMT_ex(1:6), 'o', 'LineWidth', 1.5, 'Color','blue');




% Add labels and title
xlabel('Settings');
ylabel('O2 Concentration (%)');
%title('');
% Add text labels
text(0.8, 99, 'I', 'FontSize', 12, 'FontWeight', 'bold')


ylim([85 100]);
yticks([87:3:100]);
% Adjust the figure properties
grid on;
hold off;


% Add labels and title
xlabel('Settings');
ylabel('O2 Concentration (%)');


% Add legend
legend('Procheck', 'Testbench', 'IMT', 'Location','southeast');

% Adjust the figure properties
grid on;
hold off;

%% Effects of Humidity on measurements

% Figure 1: Error of O2 Concentration
figure();
hold on;

% Plot error between both experiments - Exp 1 in violet
plot(averages(1:6, 1), (averages(1:6, 3) - data_IMT.o2(1:6)), '*-', 'LineWidth', 1.5, 'Color', [0.5804    0.0000    0.8275]);
ylabel('Error of O2 Concentration (%)');
ylim([0 6]);
plot(averages_ex(:, 1), (averages_ex(:, 3) - data_IMT_ex.o2(1:6)), '*-', 'LineWidth', 1.5, 'Color', [  1.0000    0.6471    0.0000]);
ylim([0 6]);

% Add labels and title
xlabel('Settings');
%title('Error of O2 Concentration (%)');

% Add legend
legend('Exp 1', 'Exp 2', 'Location', 'northeast');
text(0.8, 5.5, 'I', 'FontSize', 12, 'FontWeight', 'bold')
% Adjust the figure properties
grid on;
hold off;

% Figure 2: Humidity
figure();
hold on;

% Plot humidity - Exp 1 in violet
plot(averages(1:6, 1), data_IMT.Humidity(1:6), '*-', 'LineWidth', 1.5, 'Color', [0.5804    0.0000    0.8275]);
%Plot humidity - Exp 2 in orange
plot(averages_ex(1:6, 1), data_IMT_ex.Humidity(1:6), '*-', 'LineWidth', 1.5, 'Color', [1.0000    0.6471    0.0000]);
text(0.8, 20, 'I', 'FontSize', 12, 'FontWeight', 'bold')
% Add labels and title
xlabel('Settings');
ylabel('Humidity (%)');
ylim([0 22])
% Adjust the figure properties
grid on;
hold off;
% Add legend
legend('Exp 1', 'Exp 2', 'Location', 'northeast');

% Figure 3: Error of  flow
figure();
hold on;

% Plot error between both experiments - Exp 1 in violet
plot(averages(1:6, 1), abs(averages(1:6, 4) - data_IMT.flow(1:6)), '*-', 'LineWidth', 1.5, 'Color', [0.5804    0.0000    0.8275]);
ylabel('Error of Flow (L/min)');
plot(averages_ex(:, 1), abs(averages_ex(:, 4) - data_IMT_ex.flow(1:6)), '*-', 'LineWidth', 1.5, 'Color', [  1.0000    0.6471    0.0000]);
%ylim([0 6]);

% Add labels and title
xlabel('Settings');
%title('Error of O2 Concentration (%)');

% Add legend
legend('Exp 1', 'Exp 2', 'Location', 'northeast');
text(0.8, 0.15, 'I', 'FontSize', 12, 'FontWeight', 'bold')
% Adjust the figure properties
grid on;
hold off;



% Figure 3: Error of  flow
figure();
hold on;

% Plot error between both experiments - Exp 1 in violet
plot(averages(1:6, 1), (averages(1:6, 5) - data_IMT.temp(1:6)), '*-', 'LineWidth', 1.5, 'Color', [0.5804    0.0000    0.8275]);
ylabel('Error of Temperature (°C)');
plot(averages_ex(:, 1), abs((averages_ex(:, 5) - data_IMT_ex.temp(1:6))), '*-', 'LineWidth', 1.5, 'Color', [  1.0000    0.6471    0.0000]);
ylim([0 3]);

% Add labels and title
xlabel('Settings');
%title('Error of O2 Concentration (%)');

% Add legend
legend('Exp 1', 'Exp 2', 'Location', 'northeast');
text(0.8, 2.8, 'I', 'FontSize', 12, 'FontWeight', 'bold')
% Adjust the figure properties
grid on;
hold off;


%%




% Flow Data

% Create a figure
figure();
hold on;

% Plot Dataset of TestBench with error bars
errorbar(averages_ex(:, 1),averages_ex(:, 4), error_flow_tb_ex, 'o', 'LineWidth', 1.5, 'Color','green');

% Plot Dataset of IMT with error bars
errorbar(data_IMT_ex.setting(1:6), data_IMT_ex.flow(1:6), error_flow_IMT_ex(1:6), 'o', 'LineWidth', 1.5, 'Color','blue');

% Add labels and title
xlabel('Settings');
ylabel('Flow (l/min)');
%title('System Verification Tests - Low Humidity');

% Add legend
legend('Testbench', 'IMT','Location','southeast');

% Adjust the figure properties
grid on;
hold off;

% Temperature Data

% Errors for temperature of the IMT
error_temp_IMT_ex = ac_temp_IMT*ones(1,height(data_IMT_ex));  % Error constant 

% Create a figure
figure();
hold on;

% Plot Dataset of TestBench with error bars
plot(averages_ex(:, 1),averages_ex(:, 5), 'o', 'LineWidth', 1.5, 'Color','green');

% Plot Dataset of IMT with error bars
errorbar(data_IMT_ex.setting, data_IMT_ex.temp, error_temp_IMT_ex, 'o', 'LineWidth', 1.5, 'Color','blue');

% Add labels and title
xlabel('Settings');
ylabel('Temperature (°C)');
%title('System Verification Tests - Low Humidity');

% Add legend
legend('Testbench', 'IMT');

% Adjust the figure properties
grid on;
hold off;

%% Save all generated pictures

% Specify the folder path where the pictures will be saved
folderPath = 'C:\Users\kamil\OneDrive\Documents\GitHub\Internship\PosProcessing\Results';  % Replace with your desired folder path

% Create the folder if it doesn't exist
if ~isfolder(folderPath)
    mkdir(folderPath);
end

% Loop through each figure
for figureNumber = 1:numel(findobj('Type', 'figure'))
    % Activate the figure
    figure(figureNumber);
    
    % Save the figure as an image file
    filename = sprintf('Figure_%d.png', figureNumber);  % Define the filename for each figure
    saveas(gcf, fullfile(folderPath, filename), 'png');  % Save the figure as PNG (can be changed to other image formats)
end


%% Plot flows for both sensors

figure ()
hold on
time_start = 34400;
time_end = 35200;
i =1;
databySet = rawData(rawData.setting_counter == i, :);
sigDatabySet = databySet((databySet.autotime_counter >= time_start)&(databySet.autotime_counter <= time_end), :);
% Constrants of the adquisition
Ts_RMS = 1/41.72;  % seconds <-- RMS sampling rate: 41.72 Hz

time     =Ts_RMS.*(1:height(sigDatabySet));
plot(time,-sigDatabySet.gfFlowSMF, '-', 'LineWidth', 1, 'Color','green');

ylim([-0.2 12])
xlim([0 11])
grid on
ylabel ('Flow (L/min)')
xlabel ('Time (s)')



