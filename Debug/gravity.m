k=23;t=23;
load('data.txt');
load('datatrue.txt');
subplot(211);
scatter(1:1000,datatrue,'.');
hold on;
plot(data(k*25+t,:));
%xlabel('������λ��/m');
ylabel('�����쳣/mgal');
title('���߹۲�/�ϳ������쳣�Ա�');
legend('obs data','syn data');
m=zeros(1000,1000);
xstart=k*25+200;
ystart=t*25+200;
for i=xstart:xstart+200
    for j=ystart:ystart+200
        m(i,j)=1000;
    end
end
subplot(212);
imagesc(m);
%colorbar;
set(gca, 'YDir','reverse'); 
xlabel('��������/m');
ylabel('���/m');
title('�����쳣�ܶ�����');