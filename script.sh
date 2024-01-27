#!/bin/bash

# Function to display help
show_help() {
    echo "Usage: $0 [options]"
    echo "Options:"
    echo "  -h      Show help message"
    echo "  -d1     Drivers with the most trips"
    echo "  -d2     Drivers and the longest distance"
    echo "  -l      10 longest trips"
    echo "  -t      10 most traversed cities"
    echo "  -s      Statistics on stages"
}

# Check for help option
if [ "$1" == "-h" ]; then
    show_help
    exit 0
fi

process_d1() {

start_time=$(date +%s) 
awk -F ';' '!seen[$1, $6]++ {count[$6]++} END {for (name in count) print name ";" count[name]}' "$input_file" | 
    sort -t ';' -k2,2nr | head -10 | tac > temp/d1_output.txt
    gnuplot << EOF
        reset
        set terminal png size 800, 600 enhanced font 'Verdana,10'
        set output 'images/d1_graph.png'
        set title 'Drivers with the Most Trips'
        set ylabel 'Driver'
        set xlabel 'Number of Trips'
        set style fill solid
        set datafile separator ";"
        unset key
        myBoxWidth = 0.8
        set offsets 0,0,0.5-myBoxWidth/2.,0.5
        plot 'temp/d1_output.txt' using (0.5*\$2):0:(0.5*\$2):(myBoxWidth/2.):(\$0+1):ytic(1) with boxxy lc var notitle
EOF
end_time=$(date +%s)
elapsed_time=$((end_time - start_time - 3))
echo "Time taken: $elapsed_time seconds"
}

process_d2() {
start_time=$(date +%s) 
    awk -F ';' '{distance[$6]+=$5} END {for (d in distance) print d ";" distance[d]}' "$input_file" | sort -t ';' -k2,2nr | head -10 | tac > temp/d2_output.txt

    gnuplot << EOF
        reset
        set terminal png size 800, 600 enhanced font 'Verdana,10'
        set output 'images/d2_graph.png'
        set title 'Histogram of Driver Distances'
        set xlabel 'Total Distance Traveled'
        set ylabel 'Driver'
        set style fill solid
        set datafile separator ";"
        unset key
        myBoxWidth = 0.8
        set offsets 0,0,0.5-myBoxWidth/2.,0.5
        plot 'temp/d2_output.txt' using (0.5*\$2):0:(0.5*\$2):(myBoxWidth/2.):(\$0+1):ytic(1) with boxxy lc var notitle
EOF
end_time=$(date +%s) 
    elapsed_time=$((end_time - start_time))
    echo "Time taken: $elapsed_time seconds"
}

process_l() {
start_time=$(date +%s)  
    awk -F ';' '{trip[$1]+=$5} END {for (t in trip) print t, trip[t]}' "$input_file" | sort -k2 -nr | head -10 > temp/l_output.txt
    gnuplot -e "
        set terminal png size 800, 600 enhanced font 'Verdana,10';
        set output 'images/l_graph.png';
        set title '10 Longest Trips';
        set xlabel 'Trip Identifier';
        set ylabel 'Distance (km)';
        set yrange [0:3000];
        set ytics 0,500,3000;
        set boxwidth 0.5;
        set style data histograms;
        set style fill solid;
        plot 'temp/l_output.txt' using 2:xticlabels(1) with boxes;"
        
end_time=$(date +%s)
    elapsed_time=$((end_time - start_time))
    echo "Time taken: $elapsed_time seconds"
}

process_t() {
    start_time=$(date +%s)

gnuplot -e "
    set datafile separator ';';
    set terminal png size 800, 600 enhanced font 'Verdana,10';
    set output 'images/city_histogram.png';
    set title 'City Data Histogram';
    set xlabel 'Cities';
    set ylabel 'Counts';
    set boxwidth 0.4 relative;
    set style data histograms;
    set style histogram clustered;
    set style fill solid 1.0 border lt -1;
    set xtics rotate by -45;
    set grid ytics;
    set key outside;
    plot '$data_file' using 2:xtic(1) title 'Total Routes', \
         '' using 3:xtic(1) title 'First Town';
"
sleep 5
    end_time=$(date +%s)
    elapsed_time=$((end_time - start_time))
    echo "Time taken: $elapsed_time seconds"
}


process_s() {
start_time=$(date +%s) 

awk -F ';' '
{
    if(min[$1] == "" || $5 < min[$1]) min[$1] = $5;
    if(max[$1] == "" || $5 > max[$1]) max[$1] = $5;
    sum[$1] += $5; count[$1]++;
}
END {
    for (t in min) {
        avg[t] = sum[t] / count[t];
        print t, min[t], avg[t], max[t], max[t] - min[t];
    }
}' "$input_file" | sort -k5 -nr | head -50 > temp/stage_stats.txt

gnuplot -e "
    set terminal png size 1024, 768 enhanced font 'Verdana,10';
    set output 'images/stage_graph.png';
    set title 'Stage Statistics - Top 50';
    set xlabel 'Trip Identifier';
    set ylabel 'Distance (km)';
    set yrange [0:1000];
    set ytics 100;
    set xtics rotate by 270;
    set key outside;
    set style data lines;
    plot 'temp/stage_stats.txt' using 2:xticlabels(1) title 'Min', \
         '' using 3:xticlabels(1) title 'Average', \
         '' using 4:xticlabels(1) title 'Max';" 
end_time=$(date +%s)
    elapsed_time=$((end_time - start_time))
    echo "Time taken: $elapsed_time seconds"
}

data_file="progc/main.txt"
input_file="data/data.csv"
if [ ! -f "$input_file" ]; then
    echo "Error: $input_file not found"
    exit 1
fi

if [ ! -f progc/main ]; then
    echo "Compiling C program..."
    make -C progc || { echo "Compilation failed"; exit 1; }
fi
mkdir -p temp images

# Main processing logic
while [ "$1" != "" ]; do
    case $1 in
        -d1)
            process_d1
            ;;
        -d2)
            process_d2
            # Call a function or write logic for -d2 processing
            ;;
        -l)
            process_l
            # Call a function or write logic for -l processing
            ;;
        -t)
            process_t
            # Call a function or write logic for -t processing
            ;;
        -s)
            process_s
            # Call a function or write logic for -s processing
            ;;
        *)
            echo "Invalid option: $1"
            show_help
            exit 1
            ;;
    esac
    shift
done

exit 0

