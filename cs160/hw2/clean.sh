echo "Running Clean Script..."
find . -type f -name "*.class" -exec rm -f {} \;
rm -fr javadoc
rm -fr hw2.jar
