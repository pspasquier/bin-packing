heuristic="./ils"
instances="../instances/*"

for file in $instances
do
    file_name=$(basename "$file")
    echo $file_name
    $heuristic $file "ff"
    $heuristic $file "bf"
    $heuristic $file "wf"
    echo ""
done