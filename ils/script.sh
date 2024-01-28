heuristic="./ils"
instances="../instances/*"

for file in $instances
do
    file_name=$(basename "$file")
    echo $file_name
    $heuristic $file
    echo ""
done