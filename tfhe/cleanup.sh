## cleans up all exe's and keys
# Navigate to the directory where the script is located
cd "$(dirname "$0")"

# Remove executables
find . -type f -executable -exec rm {} +

# Remove specific data and key files
find . -name 'answer.data' -delete
find . -name 'cloud.data' -delete
find . -name '*.key' -delete

echo "Cleanup completed in $(pwd)."

