module Main where
import Data.Array.Unboxed
import Data.List
import System.Environment
import Text.Printf
import qualified Data.Foldable as F
import Data.Word
import Data.Bits

-- This records the index of the holes in a bit field
type Row = Word64

-- This generates the possible rows for given block sizes and row length
genRows :: [Int] -> Int -> [Row]
genRows xs n = map (permToRow 0 1) $ concatMap comboPerms $ combos xs n
  where
    combos [] 0 = return []
    combos [] _ = [] -- failure
    combos (x:xs) n =
      do c <- [0..(n `div` x)]
         rest <- combos xs (n - x*c)
         return (if c > 0 then (x, c):rest else rest)
    comboPerms [] = return []
    comboPerms bs =
      do (b, brest) <- choose bs
         rest <- comboPerms brest
         return (b:rest)
    choose bs = map (\(x, _) -> (x, remove x bs)) bs
    remove x (bc@(y, c):bs) =
      if x == y
         then if c > 1
                 then (x, c - 1):bs
                 else bs
         else bc:(remove x bs)
    remove _ [] = error "no item to remove"
    permToRow a _ [] = a
    permToRow a _ [_] = a
    permToRow a n (c:cs) =
      permToRow (a .|. m) m cs where m = n `shiftL` c

-- Test if two rows of blocks are compatible
-- i.e. they do not have a hole in common
rowCompat :: Row -> Row -> Bool
rowCompat x y = x .&. y == 0

-- It's a sparse matrix with boolean entries
type Matrix = Array Int [Int]
type Vector = UArray Int Word64

-- Creates a matrix of row compatibilities
compatMatrix :: [Row] -> Matrix
compatMatrix rows = listArray (1, n) $ map elts [1..n] where
  elts :: Int -> [Int]
  elts i = [j | j <- [1..n], rowCompat (arows ! i) (arows ! j)]
  arows = listArray (1, n) rows :: UArray Int Row
  n = length rows

-- Multiply matrix by vector, O(N^2)
mulMatVec :: Matrix -> Vector -> Vector
mulMatVec m v = array (bounds v)
    [(i, sum [v ! j | j <- m ! i]) | i <- [1..n]]
  where n = snd $ bounds v

initVec :: Int -> Vector
initVec n = array (1, n) $ zip [1..n] (repeat 1)

main = do
  args <- getArgs
  if length args < 3
    then putStrLn "usage: blocks WIDTH HEIGHT [BLOCKSIZE...]"
    else do
      let (width:height:sizes) = map read args :: [Int]
      printf "Width: %i\nHeight %i\nBlock lengths: %s\n" width height
             $ intercalate ", " $ map show sizes
      let rows = genRows sizes width
      let rowc = length rows
      printf "Row tilings: %i\n" rowc
      if null rows
        then return ()
        else do
          let m = compatMatrix rows
          printf "Matrix density: %i/%i\n"
                 (sum (map length (elems m))) (rowc^2)
          printf "Wall tilings: %i\n" $ sum $ elems
                  $ iterate (mulMatVec m) (initVec (length rows))
                            !! (height - 1)
