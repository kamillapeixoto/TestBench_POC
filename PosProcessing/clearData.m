function [sigData] = clearData (rawData)

cIndex1=rawData.gu16RMSIdx;

MissingData_Index1=(diff(cIndex1)>1);  % all of them different to >1
RepeatedData_Index1=(diff(cIndex1)==0);  % we need to erase the 2nd repeated index because the measurement is updatend in the microcontroller before the increment of the indexStamp

TotalMissing1=sum(MissingData_Index1)
TotalRepeated1=sum(RepeatedData_Index1)

% to remove (repeated data samples)
rawData(RepeatedData_Index1,:)=[];

sigData = rawData;

end