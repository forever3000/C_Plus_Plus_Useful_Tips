# alignas
**1) alignas(expression) must be an integral constant expression that evaluates to zero, or to a valid value for an alignment or extended alignment**

**2) Equivalent to alignas(alignof(type))**

**3) Equivalent to multiple alignas specifiers applied to the same declaration, one for each member of the parameter pack, which can be either type or non-type parameter pack.**

# Explanation
The alignas specifier may be applied to the declaration of a variable or a non-bitfield class data member, or it can be applied to the declaration or definition of a class/struct/union or enumeration. It cannot be applied to a function parameter or to the exception parameter of a catch clause.

The object or the type declared by such a declaration will have its alignment requirement equal to the strictest (largest) non-zero expression of all alignas specifiers used in the declaration, unless it would weaken the natural alignment of the type.

If the strictest (largest) alignas on a declaration is weaker than the alignment it would have without any alignas specifiers (that is, weaker than its natural alignment or weaker than alignas on another declaration of the same object or type), the program is ill-formed:
<pre><code>struct alignas(8) S {};
struct alignas(1) U { S s; }; // error: alignment of U would have been 8 without alignas(1)
</code></pre>

# Example

<pre><code>// every object of type sse_t will be aligned to 16-byte boundary
struct alignas(16) sse_t
{
    float sse_data[4];
};
 
// the array "cacheline" will be aligned to 128-byte boundary
alignas(128) char cacheline[128];
 
 
int main()
{
}
</code></pre>