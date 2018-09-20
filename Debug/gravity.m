k=23;t=23;
load('data.txt');
load('datatrue.txt');
subplot(211);
scatter(1:1000,datatrue,'.');
hold on;
plot(data(k*25+t,:));
%xlabel('接收器位置/m');
ylabel('重力异常/mgal');
title('测线观测/合成重力异常对比');
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
xlabel('测线走向/m');
ylabel('深度/m');
title('测线异常密度剖面');