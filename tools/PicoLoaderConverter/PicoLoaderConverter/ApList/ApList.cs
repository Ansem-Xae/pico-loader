namespace PicoLoaderConverter.ApList;

sealed class ApList
{
    public IReadOnlyList<ApListEntry> Entries { get; }

    public ApList(IEnumerable<ApListEntry> entries)
    {
        Entries = entries.ToArray();
    }
}
