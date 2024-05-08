import os
from datetime import datetime

def find_newest_file(directory):
  """
  Finds the newest file in the given directory and its subdirectories.

  Args:
    directory (str): The path to the directory to search.

  Returns:
    str: The path to the newest file found.
  """
  newest_file = None
  newest_date = None
  newest_mtime = 0

  # Traverse through all files and directories within the provided directory
  for root, dirs, files in os.walk(directory):
    for file in files:
      file_path = os.path.join(root, file)
      mtime = os.path.getmtime(file_path)

      # Update the newest file if the current file is newer
      if mtime > newest_mtime:
        newest_date = datetime.fromtimestamp(mtime)
        newest_file = file_path

  return newest_file, newest_date


directory_path = "./lib/"
result, newest_mtime = find_newest_file(directory_path)
if result:
  print(f"Aktualna data: {newest_mtime}")
  print(f"Najnowszy plik to: {result}")
else:
  print("Nie znaleziono plików w podanym katalogu.")
