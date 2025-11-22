namespace PicoLoaderConverter.SaveList;

sealed record SaveListEntry(
    uint GameCode,
    CardSaveType SaveType,
    byte SaveSize);
