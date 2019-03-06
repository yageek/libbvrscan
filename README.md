# libbvrscan

The library helps to detect the BVR value from scanned image.

## Pipeline

Assuming the user is providing an image as stripe containing the bvr, the following pipeline is targeted:

```markdown
Image  ---> Grayscale filter ---> Binarization --> Segmentation ---> Neural Net Recognition ---> BVR
```

### Grayscale filter

The grayscale filter is simply using the method described here in [Wikipedia](https://en.wikipedia.org/wiki/Grayscale)

### Binarization

The binarization is using the Sauvola method. The method is described in *Adaptive document binarization, Sauvola, PietikaKinen, Pattern Recognition 33 (2000) 225-236, 1999*

The algorithm is using the notion of *image integral* (*Viola, Jones, Rapid Object Detection using a Boosted Cascade of Simple Features*), which is an alias for *Summed Area Table* (*Summed-Area Tables for Texture Mapping, Crow, Computer Graphics Volume 18, Number 4, July 1984*).

### Segmentation

We are using the technique described in *Machine printed characted segmentation, an overview, Yi Lu, 1994* based on *vertical* and *horizontal projections*

A variation method using the [flood fill](https://en.wikipedia.org/wiki/Flood_fill) algorithm is included.