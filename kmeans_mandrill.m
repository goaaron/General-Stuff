%KMeans Mandrill
A = double(imread('mandrill-large.tiff'));
imshow(uint8(round(A)));

%load the smaller one
A = double(imread('mandrill-small.tiff'));
imshow(uint8(round(A)));

k_c=16;  %num clusters
kstart = zeros(k_c,3); 

%random sampling
s=size(A,1);
for i = 1:k_c
    j = random('Discrete Uniform', s, 1, 1);
    k = random('Discrete Uniform', s, 1, 1);
    kstart(i,:)= permute(A(j,k,:), [1 2 3]);
end


%K-means 
k1 = kstart;

for l = 1:100
    k2 = zeros(16,3);
    app = zeros(16,1);
    for j = 1:s
        for k =1:s
            diff = zeros(16,1);
            for o =1:16
                n = permute(A(j,k,:), [1 2 3]);
                
                dist = (k1(o,:))'-([n(1,1,1) n(1,1,2) n(1,1,3)])'; %Assignment
                diff(o) = dist'*dist;
            end
            [M, ind] = min(diff);
            app(ind) = app(ind) + 1;
             n2 = permute(A(i,j,:), [1 2 3]);
            k2(ind,:) = k2(ind,:)' + ([n2(1,1,1) n2(1,1,2) n2(1,1,3)])';
         end; 
    end;
    for p=1:16,
        if (app(p) > 0)
            k2(p,:) = k2(p,:)/app(p);
    end;
    end;
        k1 = k2;
end;

%Placing the colors
B= double(imread('mandrill-large.tiff'));
imshow(uint8(round(B)));
C = B;
sb = size(B,1);
for i=1:sb
    for j =1:sb
        diff=zeros(16,1);
        for k=1:16
            nn=permute(B(i,j,:), [ 1 2 3]);
            distB = k1(k,:)'-([nn(1,1,1) nn(1,1,2) nn(1,1,3)])';
            diffb(k)=distB'*distB;
        end
        [MB, indB] = min(diffb);
        C(i,j,:) = ipermute(k1(indB,:), [1 2 3]);
    end
end
figure(2)
imshow(unint8(round(C)))
          
