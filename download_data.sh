#!/bin/bash
# Downloads all the required datasets to run our code into the datasets directory, will create the datasets directory if it doesn't exist, downloads the tar files and un-tars them

directory_name='./datasets'

# Create Datasets directory
if [ -d "$directory_name" ]; then
echo "Directory already exists" ;
else
`mkdir -p $directory_name`;
echo "$directory_name directory is created"
fi


# List of urls for the datasets
url_1='https://vision.in.tum.de/rgbd/dataset/freiburg1/rgbd_dataset_freiburg1_xyz.tgz'


# Add them to array
declare -a arr=($url_1) # , $url_2, $url_3)

echo "Downloading files...";

for i in "${arr[@]}"
do
   echo "$i"
   wget -P $directory_name $i
done

echo "All files downloaded!"
echo "Un-tar all files..."
# Un-tar all files
all_tar="$directory_name/*.tgz"
for f in $all_tar
do
  echo $f
  tar zxf "$f" -C $directory_name
done

echo "Finished!"
