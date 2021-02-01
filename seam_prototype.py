import cv2
import sys
import matplotlib.pyplot as plt
from skimage.feature import hog
from skimage.transform import resize
from skimage import exposure
import numpy as np
import os

def load_png(name):
    images_path = "../examples"
    img_path = f"{images_path}/{name}"
    img = cv2.imread(img_path, 0)
    return img

def plot_pts_on_image(img, seams):
    plt.figure()
    for seam in seams:
        plt.scatter(seam, np.arange(len(seam)), color="red", s=2)
    plt.imshow(cv2.cvtColor(img, cv2.COLOR_RGB2BGR), cmap='gray')
    plt.show()

def plot_image(image):
    plt.imshow(image, cmap='gray')
    plt.show()

def calc_energy_matrix(img):
    cost_energy_mat = calc_e1(img)
    return cost_energy_mat

def calc_e1(img):
    laplacian = cv2.Laplacian(img, cv2.CV_64F)
    sobelx = cv2.Sobel(img, cv2.CV_64F,1,0,ksize=5)
    sobely = cv2.Sobel(img, cv2.CV_64F,0,1,ksize=5)
    
    return np.abs(sobelx)+np.abs(sobely)

def energy_hog(img):
    e_1 = calc_e1(img)

    img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    scale = min(img.shape[0]/128, img.shape[1]/64)
    resized = resize(img, (128*scale, 64*scale))

    fd, hog_mat = hog(resized, orientations=8, pixels_per_cell=(11, 11),
                    cells_per_block=(2, 2), visualize=True, multichannel=True)
   
    hog_mat_resized = resize(hog_mat, img.shape) 
    return e_1/np.amax(hog_mat_resized)

#TODO merge cumulative functions
def cumulative_vertical_mat(energy_mat):
    M = np.array(energy_mat)
    
    rows, cols = M.shape
    for i in range(1, rows):
        for j in range(cols):
            #TODO fix crossing the images' edges??
            try:
                M[i,j] = energy_mat[i,j] + min(M[i-1,j-1],M[i-1,j],M[i-1,j+1])
            except IndexError:
                #negative indices are "safe"
                M[i,j] = energy_mat[i,j] + min(M[i-1,j-1],M[i-1,j])
    return M

def cumulative_horizontal_mat(energy_mat):
    M = np.array(energy_mat)
    
    rows, cols = M.shape
    for c in range(1, cols):
        for r in range(rows):
            #TODO fix crossing the images' edges??
            try:
                M[r,c] = energy_mat[r,c] + min(M[r-1,c-1], M[r,c-1], M[r+1, c-1])
            except IndexError:
                #negative indices are "safe"
                M[r,c] = energy_mat[r,c] + min(M[r-1,c-1], M[r,c-1])
    return M

def minimal_vertical_seams(cumul_mat, seams_no=1):
    M = cumul_mat
    rows_no = M.shape[0]
    last_row = M[-1,:]
    min_indices = np.argpartition(last_row, seams_no)
    
    seams = np.zeros(rows_no*seams_no).reshape((seams_no, rows_no)).astype(int)
    seams[:, -1] = min_indices[:seams_no]
    for s in range(seams_no):
        for r in range(rows_no-2, -1, -1):
            prev_col = seams[s][r+1]
            seams[s][r] = get_valid_idx(M, r+1, prev_col)
    return seams

#def get_valid_energy(mat, i, j, down_direction=True):
#    direct = -1 if down_direction else 1
#    if j==0:
#        vals=[mat[i+direct, j], mat[i+direct, j+1]]
#    elif j==mat.shape[1]-1:
#        vals=[mat[i+direct, j], mat[i+direct, j-1]]
#    else:
#        vals=[mat[i+direct, j], mat[i+direct, j-1], mat[i+direct, j+1]]
#    return vals
    
def minimal_seams(e_mat):
    seam = []
    rows = e_mat.shape[0]
    
    min_row_col = rows-1, np.argmin(e_mat[-1, :])
    seam.append(min_row_col[::-1])
    #find from bottom to top
    for row in range(rows-1, 0, -1):
        curr_row, curr_col = min_row_col
        min_row_col = curr_row-1, get_valid_idx(e_mat, curr_row, curr_col)
        seam.append(min_row_col[::-1])
    return np.array(seam)

def get_valid_idx(e_mat, curr_row, curr_col):
    real_idx = curr_col
    if curr_col==0:
        idx = np.argmin(e_mat[curr_row-1][curr_col:curr_col+2])
        real_idx = curr_col if idx==0 else curr_col+1
    elif curr_col==e_mat.shape[1]-1:
        idx = np.argmin(e_mat[curr_row-1][curr_col-1:curr_col+2])
        real_idx = curr_col if idx==1 else curr_col-1
    else:
        idx = np.argmin(e_mat[curr_row-1][curr_col-1:curr_col+2])
        if idx==2:
            real_idx = curr_col+1
        if idx==0:
            real_idx = curr_col-1
    return real_idx 

def get_carved_mat(mat, seam):
    new_img = np.zeros((mat.shape[0], mat.shape[1]-1))
    for row,col in enumerate(seam):
        del_p_col = col
        del_p_row = row
        mat[del_p_row][del_p_col:] = np.roll(mat[del_p_row][del_p_col:], -1)
    return mat[:,:-1]

def main():
    img = load_png("castle.jpg")
    energy_mat = calc_energy_matrix(img)
    cumul_mat = cumulative_vertical_mat(energy_mat)
    
    removed_seams=900
    carved_mat = img
    for i in range(removed_seams):
        seams = minimal_vertical_seams(cumul_mat, 1)
        carved_mat = get_carved_mat(carved_mat, seams[0])
        cumul_mat = get_carved_mat(cumul_mat, seams[0])
    plot_pts_on_image(carved_mat, [[0]])
        
    
if __name__ == "__main__":
    main()

    
