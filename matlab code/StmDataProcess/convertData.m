[data1,data2,data3,data4]=textread('yaw.txt','%s %s %s %s');
data=[data1 data2 data3 data4];
for i = 1:4
    yaw(:,i) = hex2dec(cell2mat(data(:,i)));
end
  