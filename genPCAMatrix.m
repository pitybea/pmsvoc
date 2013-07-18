function genPCAMatrix()


features=load ('allFeatures.txt');
[z,y,yy]=princomp(features','econ');


fea=fopen('pcaMatrixSize.txt','w');
fprintf(fea,'%d %d ',size(y,1),size(y,2));
fclose(fea);

fea=fopen('pcaMatrix.txt','w');
n=size(y,1);
for i=1:n
    fprintf(fea,'%f ',y(i,:));
    fprintf(fea,'\n');
end


fclose(fea);

end