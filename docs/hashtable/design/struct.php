
<div style="width:700px;">
<!-- HTML generated using hilite.me --><div style="background: #272822; overflow:auto;width:auto;border:solid gray;border-width:.1em .1em .1em .8em;padding:.2em .6em;"><pre style="margin: 0; line-height: 125%"><span style="color: #66d9ef">typedef</span> <span style="color: #66d9ef">struct</span> <span style="color: #f8f8f2">_GroupBucket</span> <span style="color: #f8f8f2">GroupBucket;</span>
<span style="color: #66d9ef">typedef</span> <span style="color: #66d9ef">struct</span> <span style="color: #f8f8f2">_HtBucket</span>    <span style="color: #f8f8f2">HtBucket;</span>
<span style="color: #66d9ef">typedef</span> <span style="color: #66d9ef">struct</span> <span style="color: #f8f8f2">_HashTable</span>   <span style="color: #f8f8f2">HashTable;</span>
<span style="color: #66d9ef">typedef</span> <span style="color: #66d9ef">struct</span> <span style="color: #f8f8f2">_HtItem</span>      <span style="color: #f8f8f2">HtItem;</span>
<span style="color: #66d9ef">typedef</span> <span style="color: #a6e22e">size_t</span> <span style="color: #f8f8f2">(</span><span style="color: #f92672">*</span><span style="color: #f8f8f2">HashFunc)(</span><span style="color: #66d9ef">const</span> <span style="color: #66d9ef">void</span> <span style="color: #f92672">*</span><span style="color: #66d9ef">restrict</span> <span style="color: #f8f8f2">key,</span> <span style="color: #66d9ef">size_t</span> <span style="color: #f8f8f2">len);</span>

<span style="color: #66d9ef">struct</span> <span style="color: #f8f8f2">_HtItem</span>
<span style="color: #f8f8f2">{</span>
  <span style="color: #66d9ef">size_t</span>      <span style="color: #f8f8f2">klen;</span>       <span style="color: #75715e">/* The length of the key.              */</span>
  <span style="color: #66d9ef">size_t</span>      <span style="color: #f8f8f2">vlen;</span>       <span style="color: #75715e">/* The length of the value.            */</span>
  <span style="color: #66d9ef">char</span>        <span style="color: #f8f8f2">data[</span><span style="color: #ae81ff">1</span><span style="color: #f8f8f2">];</span>    <span style="color: #75715e">/* Struct hack contains key + val.     */</span>
<span style="color: #f8f8f2">};</span>

<span style="color: #66d9ef">struct</span> <span style="color: #f8f8f2">_GroupBucket</span> 
<span style="color: #f8f8f2">{</span>
  <span style="color: #f8f8f2">GroupBucket</span>  <span style="color: #f92672">*</span><span style="color: #f8f8f2">next;</span>     <span style="color: #75715e">/* Next group.                         */</span>
  <span style="color: #66d9ef">size_t</span>       <span style="color: #f8f8f2">num_item;</span>  <span style="color: #75715e">/* The number of items in this bucket. */</span>
  <span style="color: #66d9ef">char</span>         <span style="color: #f8f8f2">items[</span><span style="color: #ae81ff">1</span><span style="color: #f8f8f2">];</span>  <span style="color: #75715e">/* Struct hack contains items.         */</span>
<span style="color: #f8f8f2">};</span>

<span style="color: #66d9ef">struct</span> <span style="color: #f8f8f2">_HtBucket</span>
<span style="color: #f8f8f2">{</span>
  <span style="color: #66d9ef">size_t</span>       <span style="color: #f8f8f2">item_size;</span> <span style="color: #75715e">/* The number of items in this bucket.  */</span>
  <span style="color: #f8f8f2">GroupBucket</span>  <span style="color: #f92672">*</span><span style="color: #f8f8f2">group;</span>    <span style="color: #75715e">/* The first group of this bucket.      */</span>
<span style="color: #f8f8f2">};</span>

<span style="color: #66d9ef">struct</span> <span style="color: #f8f8f2">_HashTable</span>
<span style="color: #f8f8f2">{</span>
  <span style="color: #66d9ef">size_t</span>    <span style="color: #f8f8f2">n;</span>            <span style="color: #75715e">/* The number of items in this hashtable.       */</span>
  <span style="color: #66d9ef">size_t</span>    <span style="color: #f8f8f2">group_size;</span>   <span style="color: #75715e">/* Max bytes of a group.                        */</span>
  <span style="color: #66d9ef">size_t</span>    <span style="color: #f8f8f2">bucket_num;</span>   <span style="color: #75715e">/* The number of buckets in this hashtable.     */</span>
  <span style="color: #f8f8f2">HashFunc</span>  <span style="color: #f8f8f2">hash_f;</span>       <span style="color: #75715e">/* The hash function.                           */</span>
  <span style="color: #f8f8f2">HtBucket</span>  <span style="color: #f92672">*</span><span style="color: #f8f8f2">buckets;</span>     <span style="color: #75715e">/* The buckets array.                           */</span>
<span style="color: #f8f8f2">};</span>
</pre></div>
</div>