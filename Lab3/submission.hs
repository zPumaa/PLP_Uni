data QuadTree = White Integer 
                | Black Integer 
                | Node QuadTree QuadTree QuadTree QuadTree 
                deriving (Eq, Show)

--Function to create a QuadTree of all white pixels
allWhite :: Integer -> QuadTree
allWhite n = White n

--Function to create a QuadTree of all black pixels
allBlack :: Integer -> QuadTree
allBlack n = Black n

--Function to give a quadtree a clockwise rotation
clockwise :: QuadTree -> QuadTree -> QuadTree -> QuadTree -> QuadTree
clockwise a b c d = Node a b c d

--Function to give a quadtree an anticlockwise rotation
anticlockwise :: QuadTree -> QuadTree -> QuadTree -> QuadTree -> QuadTree
anticlockwise a b c d = Node a d c b

--Function to count the number of black or white nodes in a QuadTree
count :: QuadTree -> Integer
count (White _) = 1
count (Black _) = 1
count (Node a b c d) = count a + count b + count c + count d

countColor :: QuadTree -> QuadTree -> Integer
countColor color tree = count $ matchColor color tree

--Function to check whether a specific node matches a specific color
matchColor :: QuadTree -> QuadTree -> QuadTree
matchColor _ (White _) = White 1
matchColor _ (Black _) = Black 1

blur :: QuadTree -> QuadTree
blur (White n) = White n
blur (Black n) = Black n
blur (Node a b c d) = Node (blur a) (blur b) (blur c) (blur d)

--Function to switch the color of a QuadTree from white to black or vice versa
switch :: QuadTree -> QuadTree
switch (White n) = Black n
switch (Black n) = White n

--Function to check if a QuadTree is white
isWhite :: QuadTree -> Bool
isWhite (White _) = True
isWhite _ = False

--Function to check if a QuadTree is black
isBlack :: QuadTree -> Bool
isBlack (Black _) = True
isBlack _ = False

--Function to decide whether to switch the color of a Node or not
blurNode :: QuadTree -> QuadTree
blurNode (Node a b c d) =
    let
        newD = blur d
        newC = blur c
        newB = blur b
        newA = blur a
        node = Node newA newB newC newD
        blackCount = countColor (Black 1) (Node a b c d)
        whiteCount = countColor (White 1) (Node a b c d)
        black = isBlack (Node a b c d)
        doswitch = if black then whiteCount > blackCount else blackCount > whiteCount        
    in
        if doswitch then switch node else node
blurNode q = q







