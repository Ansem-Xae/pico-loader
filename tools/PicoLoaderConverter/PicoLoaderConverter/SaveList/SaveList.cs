namespace PicoLoaderConverter.SaveList;

sealed class SaveList
{
    public IReadOnlyList<SaveListEntry> Entries { get; }

    public SaveList(IEnumerable<SaveListEntry> entries)
    {
        Entries = entries.ToArray();
    }
}
