namespace PicoLoaderConverter.PatchList;

sealed class PatchList
{
    public IReadOnlyList<PatchListEntry> Entries { get; }

    public PatchList(IEnumerable<PatchListEntry> entries)
    {
        Entries = entries.ToArray();
    }
}
