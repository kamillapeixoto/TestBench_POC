function [result] = get_averages(rawData, completeData)

% Range of data to be computed
time_start = 30000;
time_start = 0;
time_end = 36000;
set_start = 1;

if (completeData == 1)
    set_end   = 20;    
else
    set_end   = 6; 
end

idx = set_start:set_end;
%% Compute the average of all variables

for i = idx
    databySet = rawData(rawData.setting_counter == i, :);
    sigDatabySet = databySet((databySet.autotime_counter >= time_start)&(databySet.autotime_counter <= time_end), :);
    time_diff = diff(sigDatabySet.x_Seconds);
    time_sum = sum(time_diff);
    volume = -sum(sigDatabySet.gfFlowSMF(1:end-1).*time_diff)/time_sum;
 %   averages(i,:) = [i*0.25, mean(sigDatabySet.gfRMSCurrent), mean(sigDatabySet.gfO2concentration), -sum(sigDatabySet.gfFlowSMF), mean(sigDatabySet.gfO2temperature)];
    averages(i,:) = [i*0.25, mean(sigDatabySet.gfRMSCurrent), mean(sigDatabySet.gfO2concentration), volume, mean(sigDatabySet.gfO2temperature)];

end

if (completeData == 1)
    avg_idx = [1 5 9 13 17 20];
    result = averages(avg_idx ,:);   
else
    averages(:,1) = [0.25 1.25 2.25 3.25 4.25 5];
    result = averages;
end


end