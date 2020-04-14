## Commiting using tig

### Navigate diffs
#### launch tig
- **s** # to see the status view
- **arrows** or **j/k** # to select the file diff to look at
- **enter** or **c** # to see changes in that file, this is the stage view
- **j/k** # to navigate up and down the file lines

#### Select specific chunks
- (go down to a specific chunk)
- **u** # to select the whole chunk
- **!** # to delete/reset the whole chunk change (you must be on the chunk header line) ; caution this change will be gone forever

#### Select specific lines
- (in a chunk on a specific line)
- **1** (one) to add the specific line to stage
- **!** and then y to delete/reset the specific line ; caution this line will be gone forever

#### To undo the staging and remove the line, select the file in the changes to be committed section
- **1** (one) to move the line back to the non-stage area
