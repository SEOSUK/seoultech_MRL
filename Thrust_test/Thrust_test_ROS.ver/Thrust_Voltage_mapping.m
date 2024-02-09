%% 데이터 입력
clc
clear

voltage = 19:1:25;
bag_file_path = {'C:\bag_ros\'};

bag_file = {'19.bag' , ... 
            '20.bag' , ...
            '21.bag' , ...
            '22.bag' , ...
            '23.bag' , ...
            '24.bag' , ...
            '25.bag'};

for i = 1:1:length(bag_file)
        bag = rosbag([bag_file_path{1}, bag_file{i}]);
        msgs_pwm = readMessages(select(bag, 'Topic', '/pwm'));
        msgs_thrust = readMessages(select(bag, 'Topic', '/thrust'));

        pwm = zeros(length(bag_file) , length(msgs_pwm));
        thrust = zeros(length(bag_file) , length(msgs_thrust));

        for j = 1:1:length(msgs_pwm)
            pwm(i,j) = 1000+10*msgs_pwm{j}.Data;
            %pwm(i,j) = j;
        end
        
        for j = 1:1:length(msgs_thrust)
            thrust(i,j) = msgs_thrust{j}.Data - 0.17*9.81;
            %thrust(i,j) = 413*voltage(1,i)*pwm(i,j)^2 + 31*voltage(1,i)*voltage(1,i)*12.125*pwm(i,j) + 3.1612 ;
        end  

        pwm_Matrix(i,:) = pwm(i,:);
        thrust_Matrix(i,:) = thrust(i,:);
end

pwm = pwm_Matrix;
thrust = thrust_Matrix;


coefficient = zeros(length(bag_file), 3);
trend_coefficient = zeros(3, 3);

for k=1:1:length(bag_file)
    coefficient(k,:) = polyfit(pwm(k,:), thrust(k,:), 2);
end

for k=1:1:3
    trend_coefficient(k,:) = polyfit(voltage, coefficient(:,k), 2);
end

clc
tag = {'pwm^2 coefficient', 'pwm^1 coefficient', 'pwm^0 coefficient'};
T = table(tag', trend_coefficient(:,1), trend_coefficient(:,2), trend_coefficient(:,3), 'VariableNames', {'CASE', 'voltage^2 coefficient', 'voltage^1 coefficient', 'voltage^0 coefficient'});
disp(T);
disp("");
disp("ex) pwm^1 coefficient & voltage^2 coefficient -> pwm의 1차항의 voltage의 2차항 계수");
disp("");


% 실험값 그래프 확인
voltage_d = [25.2, 25.2, 25.2, 25.2, 25.2, 25.2, 25.2];
pwm_d = 1000+10*[25, 37.5, 50, 62.5, 75.0, 87.5, 100];
thrust_d = [8.32, 12.89, 18.92, 26.32, 34.55, 40.87, 47.74]; 
% 예시 데이터 생성
trend_data = ones(1, 100);
x = linspace(min(voltage(:)), max(voltage(:)), length(trend_data));
y = linspace(min(pwm(:)), max(pwm(:)), length(trend_data));
Z = zeros(length(trend_data), length(trend_data));

for i = 1:length(trend_data)
    % C = trend_coefficient * [x(1, i)^2, x(1, i)^1, x(1, i)^0]';
    % z(1, i) = C' * [y(1, i)^2, y(1, i)^1, y(1, i)^0]';
    % Z(:,i) = (trend_coefficient(1,1)*x(1, i)^2 + trend_coefficient(1,2)*x(1, i) + trend_coefficient(1,3))*y(1, i)^2+...
    %          (trend_coefficient(2,1)*x(1, i)^2 + trend_coefficient(2,2)*x(1, i) + trend_coefficient(2,3))*y(1, i)+...
    %          (trend_coefficient(3,1)*x(1, i)^2 + trend_coefficient(3,2)*x(1, i) + trend_coefficient(3,3));
    %Z(:,i) = 4*y(1,i)^2 + 3*y(1,i) + 2;

    for j = 1:length(trend_data)
        C = trend_coefficient * [x(1, i)^2, x(1, i)^1, x(1, i)^0]';
        Z(i, j) = C' * [y(1, j)^2, y(1, j)^1, y(1, j)^0]';
    end

end

% 데이터 격자 생성
[X, Y] = meshgrid(x, y);
%[Z,  ] = meshgrid(Z, y);
%Z = repmat(z, numel(x), 1);

% 3D 면 그래프 그리기
figure;
surf(X', Y', Z, 'EdgeColor', 'none'); % 테두리 없는 면 그래프

% 바탕색 설정
set(gcf, 'color', 'white');

% 폰트 설정
set(gca, 'FontName', 'Times New Roman');

% 3차원 점 추가
hold on;
for i=1:1:length(voltage)
    scatter3(voltage(1,i)*ones(1, length(pwm(i,:))), pwm(i,:), thrust(i,:), 'o', 'MarkerFaceColor', 'r', 'MarkerEdgeColor', 'k');
end
scatter3(voltage_d, pwm_d, thrust_d, 'o', 'MarkerFaceColor', 'k', 'MarkerEdgeColor', 'k', 'LineWidth', 2);
hold off;

% 축 레이블과 타이틀 추가
xlabel('Voltage');
ylabel('PWM');
zlabel('Thrust');
title('3D Surface Plot of PWM, Voltage, and Thrust');

% 그리드 추가
grid on;

% 컬러바 추가
colorbar;