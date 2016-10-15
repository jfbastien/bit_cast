# Bit-casting object representations

Sample implementation for the C++ standards committee paper on <a
href="http://wg21.link/P0476">`bit_cast`</a>.

The implementation conforms to the API as described in the paper, and showcases
concept checking using:

* C++ concepts from the Concepts TS, as available in GCC 6 and later.
* SFINAE.
* `enable_if`.

The associated tests try to hit interesting / horrible uses of this feature.
