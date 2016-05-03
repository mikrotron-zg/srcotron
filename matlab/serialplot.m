s=serial('COM4');
set(s,'BaudRate',57600,'InputBufferSize',512,'Terminator', ',');
fopen(s);
s
flushinput(s);
data=[0;0;0;0;0]
for ctr=1:10
    for i=1:500
        try
            temp=fscanf(s,'%d %f %f %f %d')
            data=[data temp];
        catch
            i=i-1;
        end
    end
    ctr
end
fclose(s)