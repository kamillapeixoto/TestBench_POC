%% MATLAB Script for Post-Processing Testbench Data

%% Aceptance Test
% This script imports the data obtained from the automated tests of the P5
% and calculates the average values for each measurement. And plot the
% comparisons of the magnitude for differente compressor speeds.
% The experiments were conducted by varying the settings every 6 minutes.
% It takes approximately 5 minutes for the P5 to stabilize in the new settings,
% and the last minute of data is the one of interest for analysis.

% Kamilla Peixoto 17/07/2023

clear
clc
close all


% Read data
rawData0 = readtable('AP10_cc-0_full.xlsx');
rawData20 = readtable('AP10_cc-20.xlsx');
rawData25 = readtable('AP10_cc-25.xlsx');


% Clear repeated data
sigData0 = clearData (rawData0);
sigData20 = clearData (rawData20);
sigData25 = clearData (rawData25);


% Compute averages
averages0 = get_averages(sigData0, 1);
averages20 = get_averages(sigData20, 0);
averages25 = get_averages(sigData25, 0);


%%  Plot Graphs
%% Current
figure();
hold on
grid on

plot(averages0(:,1), averages0(:,2)- averages25(:,2), '*-', 'LineWidth', 1.5, 'Color','green');
plot(averages0(:,1), averages0(:,2) -averages20(:,2),'*-', 'LineWidth', 1.5, 'Color','blue');

legend('75%','80%', 'Location', 'southeast', 'FontSize', 12);
%title ('Current Reduction - AP = 10');
xlabel('Settings', 'FontSize', 14);
ylabel('Differential RMS Current (A)', 'FontSize', 14);
xticks(averages0(:,1));
xlim([0 5.25]);
set(gca, 'FontSize', 12);

%% O2 Concentration

figure();
hold on
grid on

plot(averages0(:,1), averages25(:,3), '*-', 'LineWidth', 1.5, 'Color','green');
plot(averages0(:,1), averages20(:,3),'*-', 'LineWidth', 1.5, 'Color','blue');
plot(averages0(:,1), averages0(:,3),'*-', 'LineWidth', 1.5, 'Color',[0.85, 0.33, 0.1]);

plot([0 5.25], [87 87], '--', 'color', 'black','LineWidth', 1.5);
plot([0 5.25], [96 96], '--', 'color', 'black', 'LineWidth', 1.5);

legend('75%','80%', '100%', 'FontSize', 12);
%title ('O2 Concentration - AP = 10');
xlabel('Settings', 'FontSize', 14);
ylabel('O2 Concentration (%)', 'FontSize', 14);
xticks(averages0(:,1));
yticks([87:3:100]);
xlim([0 5.25]);
ylim([85 100]);
set(gca, 'FontSize', 12);


%% Flow

figure();
hold on
grid on

plot(averages0(:,1), averages25(:,4), '*-', 'LineWidth', 1.5, 'Color','green');
plot(averages0(:,1), averages20(:,4),'*-', 'LineWidth', 1.5, 'Color','blue');
plot(averages0(:,1), averages0(:,4),'*-', 'LineWidth', 1.5, 'Color',[0.85, 0.33, 0.1]);

plot([0.25 5], [52.5 1050].*0.85/1000, '--', 'color', 'black', 'LineWidth', 1.5);
plot([0.25 5], [52.5 1050].*1.15/1000, '--', 'color', 'black', 'LineWidth', 1.5);

legend('75%','80%', '100%',  'Location','southeast', 'FontSize', 12);
%title ('Flow - AP = 10');
xlabel('Settings', 'FontSize', 14);
ylabel('Flow (L/min)', 'FontSize', 14);
xticks(averages0(:,1));

%yticks([87:3:100]);
xlim([0 5.25]);
%ylim([85 100]);
set(gca, 'FontSize', 12);


%% Flow with accuracy

figure();
hold on
grid on

errorbar(averages0(:, 1),averages0(:, 4), error_flow_tb, 'o', 'LineWidth', 1.5, 'Color',[0.85, 0.33, 0.1]);

plot([0.25 5], [52.5 1050].*0.85/1000, '--', 'color', 'black', 'LineWidth', 1.5);
plot([0.25 5], [52.5 1050].*1.15/1000, '--', 'color', 'black', 'LineWidth', 1.5);

%title ('Flow - AP = 10');
xlabel('Settings', 'FontSize', 14);
ylabel('Flow (L/min)', 'FontSize', 14);
xticks(averages0(:,1));

%yticks([87:3:100]);
xlim([0 5.25]);
%ylim([85 100]);
set(gca, 'FontSize', 12);


%% Temperature

figure();
hold on
grid on

plot(averages0(:,1), averages25(:,5), '*-', 'LineWidth', 1.5, 'Color','green');
plot(averages0(:,1), averages20(:,5),'*-', 'LineWidth', 1.5, 'Color','blue');
plot(averages0(:,1), averages0(:,5),'*-', 'LineWidth', 1.5, 'Color',[0.85, 0.33, 0.1]);


legend('75%','80%', '100%', 'Location','southeast', 'FontSize', 12);
%title ('Temperature - AP = 10');
xlabel('Settings', 'FontSize', 14);
ylabel('Temperature (°C)', 'FontSize', 14);
xticks(averages0(:,1));

%yticks([87:3:100]);
xlim([0 5.25]);
%ylim([85 100]);
set(gca, 'FontSize', 12);

%% Time Plot

%% Time Plot

Set = 6;
opt_data = sigData25;
color_opt = 'green';
vel_opt = '75%';


time_start = 30000;
time_end  = 36000;

databySet = sigData0(sigData0.setting_counter == 13, :);
sigDatabySet = databySet((databySet.autotime_counter >= time_start)&(databySet.autotime_counter <= time_end), :);
   
databySet_opt = opt_data(opt_data.setting_counter == Set, :);
sigDatabySet_opt = databySet_opt((databySet_opt.autotime_counter >= time_start)&(databySet_opt.autotime_counter <= time_end), :);

% Constrants of the adquisition
Ts_RMS = 1/41.72;  % seconds <-- RMS sampling rate: 41.72 Hz

time     =Ts_RMS.*(1:height(sigDatabySet));
time_opt = Ts_RMS.*(1:height(sigDatabySet_opt));

mean(sigDatabySet.gfFlowSMF)

%% Current

figure()
hold on
plot(time_opt, sigDatabySet_opt.gfRMSCurrent, 'LineWidth', 1, 'Color', color_opt);
plot(time, sigDatabySet.gfRMSCurrent, 'LineWidth', 1, 'Color',[0.85, 0.33, 0.1]);
grid on

legend(vel_opt, '100%', 'FontSize', 12);
xlabel('Time (s)', 'FontSize', 14)
ylabel('Current (A)', 'FontSize', 14);
xlim([0 60]);
set(gca, 'FontSize', 12);

%% O2 Concentration

figure()
hold on
plot(time_opt, sigDatabySet_opt.gfO2concentration, 'LineWidth', 1, 'Color', color_opt);
plot(time, sigDatabySet.gfO2concentration, 'LineWidth', 1, 'Color',[0.85, 0.33, 0.1]);
grid on

legend(vel_opt, '100%', 'FontSize', 12);
xlabel('Time (s)', 'FontSize', 14)
ylabel('O2 Concentration (%)', 'FontSize', 14);
xlim([0 60]);
set(gca, 'FontSize', 12);


%% Flow

figure()
hold on
plot(time_opt, -sigDatabySet_opt.gfFlowSMF, 'LineWidth', 1, 'Color', color_opt);
plot(time, -sigDatabySet.gfFlowSMF, 'LineWidth', 1, 'Color',[0.85, 0.33, 0.1]);
grid on

legend(vel_opt, '100%', 'FontSize', 12);
xlabel('Time (s)', 'FontSize', 14)
ylabel('Flow (L/min)', 'FontSize', 14);
xlim([0 60]);
set(gca, 'FontSize', 12);


%% Temperature


figure()
hold on
plot(time_opt, sigDatabySet_opt.gfO2temperature, 'LineWidth', 1, 'Color', color_opt);
plot(time, sigDatabySet.gfO2temperature, 'LineWidth', 1, 'Color',[0.85, 0.33, 0.1]);
grid on

legend(vel_opt, '100%', 'FontSize', 12);
xlabel('Time (s)', 'FontSize', 14)
ylabel('Temperature (°C)', 'FontSize', 14);
xlim([0 60]);
set(gca, 'FontSize', 12);

%% Save Pictures

% Specify the folder path where the pictures will be saved
folderPath = 'C:\Users\kamil\OneDrive\Documents\GitHub\Internship\PosProcessing\Results\AcceptanceTest\AutoPulse10\Presentation';  
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
