%% MATLAB Script for Post-Processing Testbench Data

%% Acceptance Test
% This script imports the data obtained from the automated tests of the P5
% and calculates the average values for each measurement. And plot the
% comparisons of the magnitude for different compressor speeds.
% The experiments were conducted by varying the settings every 6 minutes.
% It takes approximately 5 minutes for the P5 to stabilize in the new settings,
% and the last minute of data is the one of interest for analysis.

% Kamilla Peixoto 17/07/2023

clear
clc
close all

% Read data
rawData0 = readtable('AP25CC0.xlsx');
rawData20 = readtable('AP25CC20.xlsx');
rawData25 = readtable('AP25CC25.xlsx');

% Clear repeated data
sigData0 = clearData (rawData0);
sigData20 = clearData (rawData20);
sigData25 = clearData (rawData25);

% Compute averages
averages0 = get_averages(sigData0, 0);
averages20 = get_averages(sigData20, 0);
averages25 = get_averages(sigData25, 0);

%% Set Default Font Size
fontSize = 14;

%%  Plot Graphs
%% Current
figure();
hold on
grid on
% headers = {'Setting', 'RMS Current', 'O2', 'Flow', 'Temperature'};  % Headers for the columns

plot(averages0(:,1), averages0(:,2)- averages25(:,2), '*-', 'LineWidth', 1.5, 'Color','green');
plot(averages0(:,1), averages0(:,2) -averages20(:,2),'*-', 'LineWidth', 1.5, 'Color','blue');

legend('75%','80%', 'Location', 'southeast', 'FontSize', fontSize);
%title ('Current Reduction - AP = 10');
xlabel('Settings', 'FontSize', fontSize);
ylabel('Reduction of RMS Current (A)', 'FontSize', fontSize);
xticks(averages0(:,1));
xlim([0 5.25]);
set(gca, 'FontSize', fontSize);

%% O2 Concentration

figure();
hold on
grid on
% headers = {'Setting', 'RMS Current', 'O2', 'Flow', 'Temperature'};  % Headers for the columns

plot(averages0(:,1), averages25(:,3), '*-', 'LineWidth', 1.5, 'Color','green');
plot(averages0(:,1), averages20(:,3),'*-', 'LineWidth', 1.5, 'Color','blue');
plot(averages0(:,1), averages0(:,3),'*-', 'LineWidth', 1.5, 'Color',[0.85, 0.33, 0.1]);

plot([0 5.25], [87 87], '--', 'color', 'black');
plot([0 5.25], [96 96], '--', 'color', 'black');

legend('75%','80%', '100%', 'FontSize', fontSize, 'Location', 'northeast');
%title ('O2 Concentration - AP = 10');
xlabel('Settings', 'FontSize', fontSize);
ylabel('O2 Concentration (%)', 'FontSize', fontSize);
xticks(averages0(:,1));
yticks([87:3:100]);
xlim([0 5.25]);
ylim([85 100]);
set(gca, 'FontSize', fontSize);

%% Flow

figure();
hold on
grid on
% headers = {'Setting', 'RMS Current', 'O2', 'Flow', 'Temperature'};  % Headers for the columns

plot(averages0(:,1), averages25(:,4), '*-', 'LineWidth', 1.5, 'Color','green');
plot(averages0(:,1), averages20(:,4),'*-', 'LineWidth', 1.5, 'Color','blue');
plot(averages0(:,1), averages0(:,4),'*-', 'LineWidth', 1.5, 'Color',[0.85, 0.33, 0.1]);

plot(linspace(0, 5.25, 6), [4.7 23.5 42.4 61.3 80.2 94.5]/100, '--', 'color', 'black');
plot(linspace(0, 5.25, 6), [5.7 28.8 51.9 75 98.1 115.5]/100, '--', 'color', 'black');

legend('75%','80%', '100%', 'FontSize', fontSize, 'Location','southeast');
%title ('Flow - AP = 10');
xlabel('Settings', 'FontSize', fontSize);
ylabel('Flow (L/min)', 'FontSize', fontSize);
xticks(averages0(:,1));
set(gca, 'FontSize', fontSize);
xlim([0 5.25]);
%ylim([85 100]);

%% Flow with accuracy
figure();
hold on
grid on
% headers = {'Setting', 'RMS Current', 'O2', 'Flow', 'Temperature'};  % Headers for the columns

% Errors for flow of the testbench
ac_flow_tb = 0.0301;                     % From datasheet
error_flow_tb = 0.1 + ac_flow_tb*averages0(:, 4);  %  

% Plot Dataset of TestBench with error bars
errorbar(averages25(:, 1),averages25(:, 4), error_flow_tb, 'o', 'LineWidth', 1.5, 'Color','green');
% Plot Dataset of TestBench with error bars
errorbar(averages0(:, 1),averages0(:, 4), error_flow_tb, 'o', 'LineWidth', 1.5, 'Color',[0.85, 0.33, 0.1]);

plot([0.25 5], [52.5 1050].*0.85/1000, '--', 'color', 'black');
plot([0.25 5], [52.5 1050].*1.15/1000, '--', 'color', 'black');
legend('75%', '100%',  'FontSize', fontSize, 'Location','southeast');

%title ('Flow - AP = 25');
xlabel('Settings', 'FontSize', fontSize);
ylabel('Flow (L/min)', 'FontSize', fontSize);
xticks(averages0(:,1));
set(gca, 'FontSize', fontSize);
xlim([0 5.25]);
%ylim([85 100]);

%% Temperature

figure();
hold on
grid on
% headers = {'Setting', 'RMS Current', 'O2', 'Flow', 'Temperature'};  % Headers for the columns

plot(averages0(:,1), averages25(:,5), '*-', 'LineWidth', 1.5, 'Color','green');
plot(averages0(:,1), averages20(:,5),'*-', 'LineWidth', 1.5, 'Color','blue');
plot(averages0(:,1), averages0(:,5),'*-', 'LineWidth', 1.5, 'Color',[0.85, 0.33, 0.1]);

legend('75%','80%', '100%', 'FontSize', fontSize, 'Location','southeast');
%title ('Temperature - AP = 10');
xlabel('Settings', 'FontSize', fontSize);
ylabel('Temperature (°C)', 'FontSize', fontSize);
xticks(averages0(:,1));
set(gca, 'FontSize', fontSize);
xlim([0 5.25]);
%ylim([85 100]);

%% Time Plot

Set = 4;
opt_data = sigData25;
color_opt = 'green';
vel_opt = '75%';

time_start = 30000;
time_end  = 36000;

databySet = sigData0(sigData0.setting_counter == 4, :);
sigDatabySet = databySet((databySet.autotime_counter >= time_start)&(databySet.autotime_counter <= time_end), :);

databySet_opt = opt_data(opt_data.setting_counter == Set, :);
sigDatabySet_opt = databySet_opt((databySet_opt.autotime_counter >= time_start)&(databySet_opt.autotime_counter <= time_end), :);

% Constraints of the acquisition
Ts_RMS = 1/41.72;  % seconds <-- RMS sampling rate: 41.72 Hz

time = Ts_RMS.*(1:height(sigDatabySet));
time_opt = Ts_RMS.*(1:height(sigDatabySet_opt));

%% Current

figure()
hold on
plot(time_opt, sigDatabySet_opt.gfRMSCurrent, 'LineWidth', 1, 'Color', color_opt);
plot(time, sigDatabySet.gfRMSCurrent, 'LineWidth', 1, 'Color', [0.85, 0.33, 0.1]);
grid on

legend(vel_opt, '100%', 'FontSize', fontSize);
xlabel('Time (s)', 'FontSize', fontSize)
ylabel('Current (A)', 'FontSize', fontSize);
xlim([0 60]);
set(gca, 'FontSize', fontSize);

%% O2 Concentration

figure()
hold on
plot(time_opt, sigDatabySet_opt.gfO2concentration, 'LineWidth', 1, 'Color', color_opt);
plot(time, sigDatabySet.gfO2concentration, 'LineWidth', 1, 'Color', [0.85, 0.33, 0.1]);
grid on

ylim([85 100]);
yticks([87:3:100]);

plot([0 60], [87 87], '--', 'color', 'black');
plot([0 60], [96 96], '--', 'color', 'black');

legend(vel_opt, '100%', 'FontSize', fontSize);
xlabel('Time (s)', 'FontSize', fontSize)
ylabel('O2 Concentration (%)', 'FontSize', fontSize);
xlim([0 60]);
set(gca, 'FontSize', fontSize);

%% Flow

figure()
hold on
plot(time_opt, -sigDatabySet_opt.gfFlowSMF, 'LineWidth', 1, 'Color', color_opt);
plot(time, -sigDatabySet.gfFlowSMF, 'LineWidth', 1, 'Color', [0.85, 0.33, 0.1]);
grid on

legend(vel_opt, '100%', 'FontSize', fontSize);
xlabel('Time (s)', 'FontSize', fontSize)
ylabel('Flow (L/min)', 'FontSize', fontSize);
xlim([0 60]);
set(gca, 'FontSize', fontSize);

%% Temperature

figure()
hold on
plot(time_opt, sigDatabySet_opt.gfO2temperature, 'LineWidth', 1, 'Color', color_opt);
plot(time, sigDatabySet.gfO2temperature, 'LineWidth', 1, 'Color', [0.85, 0.33, 0.1]);
grid on

legend(vel_opt, '100%', 'FontSize', fontSize);
xlabel('Time (s)', 'FontSize', fontSize)
ylabel('Temperature (°C)', 'FontSize', fontSize);
xlim([0 60]);
set(gca, 'FontSize', fontSize);

%% Save Pictures

% Specify the folder path where the pictures will be saved
folderPath = 'C:\Users\kamil\OneDrive\Documents\GitHub\Internship\PosProcessing\Results\AcceptanceTest\AutoPulse25\Presentation';  

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
